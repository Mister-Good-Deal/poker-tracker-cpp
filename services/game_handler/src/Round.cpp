#include "game_handler/Round.hpp"

#include <utility>

#include <ranges>

namespace GameHandler {
    using std::chrono::duration_cast;
    using std::chrono::seconds;
    using std::ranges::find_if;
    using std::ranges::for_each;
    using std::ranges::sort;

    using enum Round::Street;
    using enum RoundAction::ActionType;

    Round::Round(const Blinds& blinds, std::array<Player, 3>& players) :
        _blinds(blinds), _players(&players), _lastActionTime(system_clock::now()), _hand((players)[0].getHand()),
        _playersStatus(std::make_unique<players_status_t>(
            players_status_t{{PlayerStatus{players[0]}, PlayerStatus{players[1]}, PlayerStatus{players[2]}}})) {}

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

    auto Round::call(uint8_t playerNum, int32_t amount) -> void {
        auto& player = _getPlayer(playerNum);

        _actions.at(_currentStreet).emplace_back(CALL, player, _getAndResetLastActionTime(), amount);
        _getPlayerStatus(playerNum).hasCalled(amount);
        _pot += amount;
        _processStreet(CALL);
    }

    auto Round::bet(uint8_t playerNum, int32_t amount) -> void {
        auto& player = _getPlayer(playerNum);

        _actions.at(_currentStreet).emplace_back(BET, player, _getAndResetLastActionTime(), amount);
        _getPlayerStatus(playerNum).hasBet(amount);
        _lastAction = BET;
        _pot += amount;
    }

    auto Round::check(uint8_t playerNum) -> void {
        auto& player = _getPlayer(playerNum);

        _actions.at(_currentStreet).emplace_back(CHECK, player, _getAndResetLastActionTime());
        _getPlayerStatus(playerNum).hasChecked();
        _processStreet(CHECK);
    }

    auto Round::fold(uint8_t playerNum) -> void {
        auto& player = _getPlayer(playerNum);

        _actions.at(_currentStreet).emplace_back(FOLD, player, _getAndResetLastActionTime());
        _getPlayerStatus(playerNum).hasFolded();
        _ranking.emplace(std::vector<Player>{player});
        _processStreet(FOLD);
        _determineRoundOver();
    }

    auto Round::showdown() -> void {
        // @todo Implement showdown
        Hand bestHand = Hand();

        //        for_each(_playersStatus.begin(), _playersStatus.end(), [&](const PlayerStatus& playerStatus) {
        //            if (playerStatus.inRound) { hands[playerStatus.playerNum] = _getPlayer(playerStatus.playerNum).getHand(); }
        //        });
        //
        //        auto haveHandSet  = [&](const auto& pair) { return pair.second.isSet(); };
        //        auto winnerFinder = [&](const auto& pair) { return pair.second > hands.at(_winner->getNumber()); };
        //
        //        if (!std::all_of(hands.begin(), hands.end(), haveHandSet)) { throw std::runtime_error("Not all players have set their
        //        hand"); }
        //
        //        _winner = &_getPlayer(std::max_element(hands.begin(), hands.end(), winnerFinder)->first);
        //        _ended  = true;
    }

    auto Round::toJson() const -> json {
        if (_ranking.empty()) { throw std::runtime_error("The round's ranking has not been set"); }

        auto preFlopActions = json::array();
        auto flopActions    = json::array();
        auto turnActions    = json::array();
        auto riverActions   = json::array();

        for_each(_actions[PREFLOP], [&](const RoundAction& action) { preFlopActions.emplace_back(action.toJson()); });
        for_each(_actions[FLOP], [&](const RoundAction& action) { flopActions.emplace_back(action.toJson()); });
        for_each(_actions[TURN], [&](const RoundAction& action) { turnActions.emplace_back(action.toJson()); });
        for_each(_actions[RIVER], [&](const RoundAction& action) { riverActions.emplace_back(action.toJson()); });

        return {{"actions", {{"pre_flop", preFlopActions}, {"flop", flopActions}, {"turn", turnActions}, {"river", riverActions}}},
                {"board", _board.toJson()},
                {"hand", _hand.toJson()},
                {"blinds", {{"small", _blinds.SB()}, {"big", _blinds.BB()}}},
                {"pot", _pot},
                {"won", _hasWon()},
                {"ranking", _toJson(_ranking)}};
    }

    auto Round::_getPlayer(uint8_t playerNum) -> Player& {
        if (playerNum <= 0 || playerNum > 3) { throw std::invalid_argument("The given player number is invalid"); }

        return _players->at(playerNum - 1);
    }

    auto Round::_getPlayerStatus(uint8_t playerNum) -> PlayerStatus& {
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
    auto Round::_processStreet(const ActionType& currentPlayerAction) -> void {
        auto IsInRound  = [](const PlayerStatus& playerStatus) { return playerStatus.inRound; };
        auto hasChecked = [](const PlayerStatus& playerStatus) { return playerStatus.lastAction == CHECK; };
        auto hasPlayed  = [](const PlayerStatus& playerStatus) {
            return !playerStatus.inRound || playerStatus.isAllIn || playerStatus.lastAction != NONE;
        };
        auto roundPlayersCount = std::count_if(_playersStatus->begin(), _playersStatus->end(), IsInRound);
        auto allPlayersPlayed  = std::all_of(_playersStatus->begin(), _playersStatus->end(), hasPlayed);

        bool currentPlayerCalled = currentPlayerAction == CALL;

        if ((currentPlayerCalled && roundPlayersCount == 2 && allPlayersPlayed)
            || (currentPlayerCalled && roundPlayersCount == 3 && allPlayersPlayed && _lastAction == CALL)
            || std::all_of(_playersStatus->begin(), _playersStatus->end(), hasChecked)) {
            _endStreet();
        } else {
            _lastAction = currentPlayerAction;
        }
    }

    auto Round::_determineRoundOver() -> void {
        auto winnerFinder = [](const PlayerStatus& playerStatus) { return playerStatus.inRound; };

        if (std::count_if(_playersStatus->begin(), _playersStatus->end(), winnerFinder) == 1) {
            _processRanking();
            _ended = true;
        }
    }

    auto Round::_endStreet() -> void {
        switch (_currentStreet) {
            case PREFLOP: _currentStreet = FLOP; break;
            case FLOP: _currentStreet = TURN; break;
            case TURN: _currentStreet = RIVER; break;
            case RIVER: _currentStreet = SHOWDOWN; break;
            case SHOWDOWN: break;
        }
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
}  // namespace GameHandler