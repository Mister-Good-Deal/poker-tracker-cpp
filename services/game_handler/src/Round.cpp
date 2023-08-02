#include "game_handler/Round.hpp"

#include <utility>

#include <ranges>

namespace GameHandler {
    using std::chrono::duration_cast;
    using std::chrono::seconds;
    using std::ranges::any_of;
    using std::ranges::find_if;
    using std::ranges::for_each;
    using std::ranges::sort;

    using enum Round::Street;
    using enum RoundAction::ActionType;

    Round::Round(const Blinds& blinds, std::array<Player, 3>& players) :
        _blinds(blinds), _players(&players), _lastActionTime(system_clock::now()), _hand((players)[0].getHand()),
        _playersStatus(std::make_unique<players_status_t>(
            players_status_t{{PlayerStatus{players[0]}, PlayerStatus{players[1]}, PlayerStatus{players[2]}}})) {
        _payBlinds();
    }

    auto Round::operator=(Round&& other) noexcept -> Round& {
        if (this != &other) {
            _actions        = std::move(other._actions);
            _board          = std::move(other._board);
            _ranking        = std::move(other._ranking);
            _hand           = std::move(other._hand);
            _playersStatus  = std::move(other._playersStatus);
            _blinds         = other._blinds;
            _pot            = other._pot;
            _currentStreet  = other._currentStreet;
            _lastActionTime = other._lastActionTime;
            _players        = other._players;
            _ended          = other._ended;
            _initialized    = other._initialized;
        }

        return *this;
    }

    auto Round::call(uint32_t playerNum, int32_t amount) -> void {
        auto& player = _getPlayer(playerNum);

        _actions.at(_currentStreet).emplace_back(CALL, player, _getAndResetLastActionTime(), amount);
        _getPlayerStatus(playerNum).hasCalled(amount);
        _pot += amount;
        _streePot += amount;
        _determineStreetOver(CALL);
    }

    auto Round::bet(uint32_t playerNum, int32_t amount) -> void {
        auto& player = _getPlayer(playerNum);

        _actions.at(_currentStreet).emplace_back(BET, player, _getAndResetLastActionTime(), amount);
        _getPlayerStatus(playerNum).hasBet(amount);
        _lastAction = BET;
        _pot += amount;
        _streePot += amount;
    }

    auto Round::check(uint32_t playerNum) -> void {
        auto& player = _getPlayer(playerNum);

        _actions.at(_currentStreet).emplace_back(CHECK, player, _getAndResetLastActionTime());
        _getPlayerStatus(playerNum).hasChecked();
        _determineStreetOver(CHECK);
    }

    auto Round::fold(uint32_t playerNum) -> void {
        auto& player = _getPlayer(playerNum);

        _actions.at(_currentStreet).emplace_back(FOLD, player, _getAndResetLastActionTime());
        _getPlayerStatus(playerNum).hasFolded();
        _ranking.emplace(std::vector<Player>{player});
        _determineStreetOver(FOLD);
        _determineRoundOver();
    }

    auto Round::showdown() -> void {
        if (_currentStreet != SHOWDOWN) { throw std::runtime_error("The round is not over yet"); }

        _endStreet();
        // @todo put this in a function
        _processRanking();
        _updateStacks();
        _ended = true;
    }

    auto Round::toJson() const -> json {
        if (_ranking.empty()) { throw std::runtime_error("The round's ranking has not been set"); }

        auto preFlopActions = json::array();
        auto flopActions    = json::array();
        auto turnActions    = json::array();
        auto riverActions   = json::array();
        auto hands          = json::object();

        for_each(_actions[PREFLOP], [&](const RoundAction& action) { preFlopActions.emplace_back(action.toJson()); });
        for_each(_actions[FLOP], [&](const RoundAction& action) { flopActions.emplace_back(action.toJson()); });
        for_each(_actions[TURN], [&](const RoundAction& action) { turnActions.emplace_back(action.toJson()); });
        for_each(_actions[RIVER], [&](const RoundAction& action) { riverActions.emplace_back(action.toJson()); });
        for_each(*_players, [&](const Player& player) { hands.emplace(player.getName(), player.getHand().toJson()); });

        return {{"actions", {{"pre_flop", preFlopActions}, {"flop", flopActions}, {"turn", turnActions}, {"river", riverActions}}},
                {"board", _board.toJson()},
                {"hands", hands},
                {"blinds", {{"small", _blinds.SB()}, {"big", _blinds.BB()}}},
                {"pot", _pot},
                {"won", _hasWon()},
                {"stacks", _getStacksVariation()},
                {"ranking", _toJson(_ranking)}};
    }

    auto Round::_getPlayer(uint32_t playerNum) -> Player& {
        if (playerNum <= 0 || playerNum > 3) { throw std::invalid_argument("The given player number is invalid"); }

        return _players->at(playerNum - 1);
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

    auto Round::_getPlayerStatus(uint32_t playerNum) -> PlayerStatus& {
        if (playerNum <= 0 || playerNum > 3) { throw std::invalid_argument("The given player number is invalid"); }

        return _playersStatus->at(playerNum - 1);
    }

    auto Round::_getPlayerStatus(uint32_t playerNum) const -> PlayerStatus {
        if (playerNum <= 0 || playerNum > 3) { throw std::invalid_argument("The given player number is invalid"); }

        return _playersStatus->at(playerNum - 1);
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

    // @todo Regarder si _determineRoundOver ne peut pas être appelé dans _endStreet
    auto Round::_determineRoundOver() -> void {
        auto winnerFinder = [](const PlayerStatus& playerStatus) { return playerStatus.inRound; };

        if (std::count_if(_playersStatus->begin(), _playersStatus->end(), winnerFinder) == 1) {
            // @todo put this in a function
            _processRanking();
            _updateStacks();
            _ended = true;
        }
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
        _streePot   = 0;
        _frozenPot  = _pot;

        for (auto& playerStatus : *_playersStatus) { playerStatus.streetReset(); }
    }

    auto Round::_hasWon() const -> bool {
        auto rankFirst = _ranking.top();

        return find_if(rankFirst, [&](const Player& player) { return player.getNumber() == 1; }) != rankFirst.end();
    }

    auto Round::_processRanking() -> void {
        std::vector<Player> players;
        players.reserve(_playersStatus->size());

        for (const auto& playerStatus : *_playersStatus) {
            if (playerStatus.inRound) { players.push_back(_getPlayer(playerStatus.playerNum)); }
        }

        sort(players, [&](const Player& p1, const Player& p2) { return _board.compareHands(p1.getHand(), p2.getHand()); });

        // iterate through the players from last to first and add them to the _ranking stack
        for (auto& player : std::ranges::reverse_view(players)) {
            if (_ranking.empty() || _board.compareHands(player.getHand(), _ranking.top().front().getHand()) == 0) {
                _ranking.top().emplace_back(player);
            } else {
                _ranking.emplace(std::vector<Player>{player});
            }
        }
    }

    auto Round::_payBlinds() -> void {
        auto dealer = find_if(*_players, [&](const Player& player) { return player.isDealer(); });

        if (dealer == _players->end()) { throw std::runtime_error("No dealer found"); }

        auto  dealerNum = dealer->getNumber();
        auto& SBPlayer  = _getPlayerStatus(_getNextPlayerNum(dealerNum));
        auto& BBPlayer  = _getPlayerStatus(_getNextPlayerNum(SBPlayer.playerNum));  // Can be the dealer if there are only 2 players

        SBPlayer.payBlind(_blinds.SB());
        BBPlayer.payBlind(_blinds.BB());
        // @todo check the rule for all in players when paying blinds
        _streePot += SBPlayer.totalBet + BBPlayer.totalBet;
        _pot = _streePot;
    }

    auto Round::_updateStacks() -> void {
        // Use a copy of _ranking and _pot because it will be modified
        auto ranking = _ranking;
        auto pot     = _pot;
        // Add pot to winner(s) stack
        while (!ranking.empty()) {
            auto& players = ranking.top();
            // Sort players by stacks asc and pay players by this order
            sort(players, [&](const Player& p1, const Player& p2) { return p1.getStack() < p2.getStack(); });
            // Number of remaining players to share the pot
            auto remainingPlayers = static_cast<int32_t>(players.size());

            for (auto& player : players) {
                auto& playerStatus = _getPlayerStatus(player.getNumber());
                auto  winAmount    = std::min(playerStatus.maxWinnable, pot / remainingPlayers--);

                playerStatus.won = winAmount;
                player.winStack(winAmount);
                pot -= winAmount;
            }

            ranking.pop();
        }

        // Retrieve players round bets to their stack
        for (const auto& playerStatus : *_playersStatus) {
            _getPlayer(playerStatus.playerNum).loseStack(playerStatus.totalBet - playerStatus.won);
        }
    }

    /**
     * @brief Update the max winnable amount for each player at the end of a street
     */
    auto Round::_updatePlayersMaxWinnable() -> void {
        for (auto& playerStatus : *_playersStatus) {
            if (!playerStatus.isAllIn) {
                playerStatus.maxWinnable = _frozenPot + _streePot;
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

            for (const auto& player : rankStep) { rankStepJson.emplace_back(player.getName()); }

            rankingJson.emplace_back(rankStepJson);
            rankingCopy.pop();
        }

        return rankingJson;
    }

    auto Round::_getStacksVariation() const -> json {
        auto playersStack = json::array();

        for (const auto& player : *_players) {
            playersStack.emplace_back(
                json::object({{"player", player.getName()},
                              {"stack", player.getStack()},
                              {"balance", player.getStack() - _getPlayerStatus(player.getNumber()).initialStack}}));
        }

        return playersStack;
    }
}  // namespace GameHandler