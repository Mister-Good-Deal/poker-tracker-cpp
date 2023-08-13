#include "game_handler/Round.hpp"

#include <utility>

#include <ranges>

namespace GameHandler {
    using fmt::format;
    using std::chrono::duration_cast;
    using std::chrono::seconds;
    using std::ranges::any_of;
    using std::ranges::find_if;
    using std::ranges::for_each;
    using std::ranges::sort;

    using enum Round::Street;
    using enum RoundAction::ActionType;

    Round::Round(const Blinds& blinds, std::array<Player, 3>& players, Hand hand, int32_t dealerNumber) :
        _blinds(blinds), _players(&players), _lastActionTime(system_clock::now()),
        _playersStatus(std::make_unique<players_status_t>(
            players_status_t{{PlayerStatus{players[0], dealerNumber}, PlayerStatus{players[1], dealerNumber},
                              PlayerStatus{players[2], dealerNumber}}})) {
        _getPlayerStatus(1).hand = std::move(hand);
        _payBlinds();
    }

    auto Round::operator=(Round&& other) noexcept -> Round& {
        if (this != &other) {
            _actions        = std::move(other._actions);
            _board          = std::move(other._board);
            _ranking        = std::move(other._ranking);
            _playersStatus  = std::move(other._playersStatus);
            _blinds         = other._blinds;
            _pot            = other._pot;
            _currentStreet  = other._currentStreet;
            _lastActionTime = other._lastActionTime;
            _players        = other._players;
            _ended          = other._ended;
        }

        return *this;
    }

    auto Round::call(uint32_t playerNum, int32_t amount) -> void {
        auto& player = _getPlayerStatus(playerNum);

        _actions.at(_currentStreet).emplace_back(CALL, player, _getAndResetLastActionTime(), amount);
        player.hasCalled(amount);
        _pot += amount;
        _streetPot += amount;
        _determineStreetOver(CALL);
    }

    auto Round::bet(uint32_t playerNum, int32_t amount) -> void {
        auto& player = _getPlayerStatus(playerNum);

        _actions.at(_currentStreet).emplace_back(BET, player, _getAndResetLastActionTime(), amount);
        player.hasBet(amount);
        _lastAction = BET;
        _pot += amount;
        _streetPot += amount;
    }

    auto Round::raise(uint32_t playerNum, int32_t amount) -> void {
        auto& player = _getPlayerStatus(playerNum);

        _actions.at(_currentStreet).emplace_back(RAISE, player, _getAndResetLastActionTime(), amount);
        player.hasRaised(amount);
        _lastAction = RAISE;
        _pot += amount;
        _streetPot += amount;
    }
    
    auto Round::check(uint32_t playerNum) -> void {
        auto& player = _getPlayerStatus(playerNum);

        _actions.at(_currentStreet).emplace_back(CHECK, player, _getAndResetLastActionTime());
        player.hasChecked();
        _determineStreetOver(CHECK);
    }

    auto Round::fold(uint32_t playerNum) -> void {
        auto& player = _getPlayerStatus(playerNum);

        _actions.at(_currentStreet).emplace_back(FOLD, player, _getAndResetLastActionTime());
        player.hasFolded();
        _ranking.emplace(std::vector<int32_t>{player.getNumber()});
        _determineStreetOver(FOLD);
    }

    auto Round::showdown() -> void {
        _endStreet();
        _endRound();
    }

    auto Round::toJson() const -> json {
        if (_ranking.empty()) { throw std::runtime_error("The round's ranking has not been set"); }

        auto preFlopActions = json::array();
        auto flopActions    = json::array();
        auto turnActions    = json::array();
        auto riverActions   = json::array();
        auto hands          = json::object();
        auto positions      = json::object();

        for_each(_actions[PREFLOP], [&](const RoundAction& action) { preFlopActions.emplace_back(action.toJson()); });
        for_each(_actions[FLOP], [&](const RoundAction& action) { flopActions.emplace_back(action.toJson()); });
        for_each(_actions[TURN], [&](const RoundAction& action) { turnActions.emplace_back(action.toJson()); });
        for_each(_actions[RIVER], [&](const RoundAction& action) { riverActions.emplace_back(action.toJson()); });

        for (const auto& player : *_playersStatus) {
            positions.emplace(format("{}", player.position), format("player_{}", player.getNumber()));
        }

        for (const auto& player : *_playersStatus) { hands.emplace(format("player_{}", player.getNumber()), player.hand.toJson()); }

        return {{"actions", {{"pre_flop", preFlopActions}, {"flop", flopActions}, {"turn", turnActions}, {"river", riverActions}}},
                {"board", _board.toJson()},
                {"hands", hands},
                {"blinds", {{"small", _blinds.SB()}, {"big", _blinds.BB()}}},
                {"pot", _pot},
                {"won", _hasWon()},
                {"positions", positions},
                {"stacks", _getStacksVariation()},
                {"ranking", _toJson(_ranking)}};
    }

    auto Round::_getPlayer(uint32_t playerNum) -> Player& {
        if (playerNum <= 0 || playerNum > 3) { throw std::invalid_argument("The given player number is invalid"); }

        return _players->at(playerNum - 1);
    }

    auto Round::_getPlayer(uint32_t playerNum) const -> Player {
        if (playerNum <= 0 || playerNum > 3) { throw std::invalid_argument("The given player number is invalid"); }

        return _players->at(playerNum - 1);
    }

    auto Round::_getPlayerStatus(uint32_t playerNum) -> PlayerStatus& {
        if (playerNum <= 0 || playerNum > 3) { throw std::invalid_argument("The given player number is invalid"); }

        return _playersStatus->at(playerNum - 1);
    }

    auto Round::_getPlayerStatus(uint32_t playerNum) const -> PlayerStatus {
        if (playerNum <= 0 || playerNum > 3) { throw std::invalid_argument("The given player number is invalid"); }

        return _playersStatus->at(playerNum - 1);
    }

    auto Round::_getNextPlayerNum(uint32_t playerNum) -> uint32_t {
        if (playerNum <= 0 || playerNum > 3) { throw std::invalid_argument("The given player number is invalid"); }

        uint32_t nextPlayerNum = (playerNum % 3) + 1;

        while (nextPlayerNum != playerNum) {
            if (_getPlayerStatus(nextPlayerNum).inRound) { return nextPlayerNum; }

            nextPlayerNum = (nextPlayerNum % 3) + 1;
        }

        throw std::runtime_error("No next player found");
    }

    auto Round::_getAndResetLastActionTime() -> seconds {
        auto now            = system_clock::now();
        auto lastActionTime = duration_cast<seconds>(now - _lastActionTime);

        _lastActionTime = now;

        return lastActionTime;
    }

    /**
     * @brief Processes the current street in the round based on the current player action.
     *
     * The function determines if the current street has ended based on the current player's action and the current state of the
     * players in the round. If the street has ended, it calls the endStreet() function. Otherwise, it updates the last action of the
     * current player.
     *
     * @param currentPlayerAction The action taken by the current player.
     * @return void
     */
    auto Round::_determineStreetOver(const ActionType& currentPlayerAction) -> void {
        auto IsInRound  = [](const PlayerStatus& playerStatus) { return playerStatus.inRound; };
        auto hasChecked = [](const PlayerStatus& playerStatus) { return !playerStatus.inRound || playerStatus.lastAction == CHECK; };
        auto hasPlayed  = [](const PlayerStatus& playerStatus) {
            return !playerStatus.inRound || playerStatus.isAllIn || playerStatus.lastAction != NONE;
        };
        auto roundPlayersCount = std::count_if(_playersStatus->begin(), _playersStatus->end(), IsInRound);
        auto allPlayersPlayed  = std::all_of(_playersStatus->begin(), _playersStatus->end(), hasPlayed);

        bool currentPlayerCalled = currentPlayerAction == CALL;

        if (roundPlayersCount == 1 || (currentPlayerCalled && roundPlayersCount == 2 && allPlayersPlayed)
            || (currentPlayerCalled && roundPlayersCount == 3 && allPlayersPlayed && _lastAction == CALL)
            || std::all_of(_playersStatus->begin(), _playersStatus->end(), hasChecked)) {
            _endStreet();
        } else {
            _lastAction = currentPlayerAction;
        }
    }

    auto Round::_determineRoundOver() -> void {
        auto winnerFinder = [](const PlayerStatus& playerStatus) { return playerStatus.inRound; };

        if (std::count_if(_playersStatus->begin(), _playersStatus->end(), winnerFinder) == 1) { _endRound(); }
    }

    auto Round::_endStreet() -> void {
        _updatePlayersMaxWinnable();

        switch (_currentStreet) {
            case PREFLOP: _currentStreet = FLOP; break;
            case FLOP: _currentStreet = TURN; break;
            case TURN: _currentStreet = RIVER; break;
            case RIVER: _currentStreet = SHOWDOWN; break;
            case SHOWDOWN: break;
        }

        _lastAction = NONE;
        _streetPot  = 0;
        _frozenPot  = _pot;

        for (auto& playerStatus : *_playersStatus) { playerStatus.streetReset(); }

        _determineRoundOver();
    }

    auto Round::_endRound() -> void {
        _processRanking();
        _updateStacks();

        _ended = true;
    }

    auto Round::_hasWon() const -> bool {
        auto rankFirst = _ranking.top();

        return find_if(rankFirst, [&](int32_t playerNum) { return playerNum == 1; }) != rankFirst.end();
    }

    auto Round::_processRanking() -> void {
        std::vector<PlayerStatus> players;
        players.reserve(_playersStatus->size());

        for (const auto& playerStatus : *_playersStatus) {
            if (playerStatus.inRound) { players.push_back(playerStatus); }
        }

        sort(players, [&](const PlayerStatus& p1, const PlayerStatus& p2) { return _board.compareHands(p1.hand, p2.hand); });

        // iterate through the players in round from last to first and add them to the _ranking stack
        for (auto& player : players) {
            if (!_ranking.empty() && _board.compareHands(player.hand, _getPlayerStatus(_ranking.top().front()).hand) == 0) {
                _ranking.top().emplace_back(player.getNumber());
            } else {
                _ranking.emplace(std::vector<int32_t>{player.getNumber()});
            }
        }
    }

    auto Round::_payBlinds() -> void {
        auto dealer = find_if(*_playersStatus, [&](const PlayerStatus& player) { return player.position == DEALER; });

        if (dealer == _playersStatus->end()) { throw std::runtime_error("No dealer found"); }

        auto& SBPlayer = _getPlayerStatus(_getNextPlayerNum(dealer->getNumber()));
        auto& BBPlayer = _getPlayerStatus(_getNextPlayerNum(SBPlayer.getNumber()));  // Can be the dealer if there are only 2 players

        SBPlayer.payBlind(_blinds.SB());
        BBPlayer.payBlind(_blinds.BB());
        // @todo check the rule for all in players when paying blinds
        _streetPot += SBPlayer.totalBet + BBPlayer.totalBet;
        _pot = _streetPot;
    }

    auto Round::_updateStacks() -> void {
        // Use a copy of _ranking and _pot because it will be modified
        auto ranking = _ranking;
        auto pot     = _pot;
        // Add pot to winner(s) stack
        while (!ranking.empty()) {
            auto playersNum = ranking.top();
            // Sort players by stacks asc and pay players by this order
            sort(playersNum, [&](int32_t p1Num, int32_t p2Num) {
                return _getPlayerStatus(p1Num).getStack() < _getPlayerStatus(p2Num).getStack();
            });
            // Number of remaining players to share the pot
            auto remainingPlayers = static_cast<int32_t>(playersNum.size());

            for (int32_t playerNum : playersNum) {
                auto& player    = _getPlayerStatus(playerNum);
                auto  winAmount = std::min(player.maxWinnable, pot / remainingPlayers--);

                player.endRoundStack += winAmount - player.totalBet;
                _getPlayer(player.getNumber()).setStack(player.endRoundStack);
                pot -= winAmount;
            }

            ranking.pop();
        }
    }

    /**
     * @brief Update the max winnable amount for each player at the end of a street
     */
    auto Round::_updatePlayersMaxWinnable() -> void {
        for (auto& playerStatus : *_playersStatus) {
            if (!playerStatus.isAllIn) {
                playerStatus.maxWinnable = _frozenPot + _streetPot;
            } else if (playerStatus.totalStreetBet != 0) {
                int32_t streetWinnable = 0;

                for (const auto& player : *_playersStatus) {
                    streetWinnable += std::min(player.totalStreetBet, playerStatus.totalStreetBet);
                }

                playerStatus.maxWinnable = _frozenPot + streetWinnable;
            }
        }
    }

    auto Round::_toJson(const ranking_t& ranking) const -> json {
        auto rankingJson = json::array();
        auto rankingCopy = ranking;

        while (!rankingCopy.empty()) {
            auto rankStepJson = json::array();
            auto rankStep     = rankingCopy.top();

            for (const auto& playerNum : rankStep) { rankStepJson.emplace_back(_getPlayerStatus(playerNum).getName()); }

            rankingJson.emplace_back(rankStepJson);
            rankingCopy.pop();
        }

        return rankingJson;
    }

    auto Round::_getStacksVariation() const -> json {
        auto playersStack = json::array();

        for (const auto& player : *_playersStatus) {
            playersStack.emplace_back(json::object({{"player", player.getName()},
                                                    {"stack", player.endRoundStack},
                                                    {"balance", player.endRoundStack - player.getStack()}}));
        }

        return playersStack;
    }
}  // namespace GameHandler