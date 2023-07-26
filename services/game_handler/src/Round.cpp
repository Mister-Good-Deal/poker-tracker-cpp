#include "game_handler/Round.hpp"

#include <utility>

namespace GameHandler {
    using std::chrono::duration_cast;
    using std::chrono::seconds;
    using std::ranges::for_each;

    using enum Round::Street;
    using enum RoundAction::ActionType;

    Round::Round(Hand hand, const Blinds& blinds, std::array<Player, 3>& players) :
        _hand(std::move(hand)), _blinds(blinds), _players(&players),
        _playersStatus{{PlayerStatus{(*_players)[0]}, PlayerStatus{(*_players)[1]}, PlayerStatus{(*_players)[2]}}} {}

    auto Round::operator=(Round&& other) noexcept -> Round& {
        if (this != &other)
        {
            _actions        = std::move(other._actions);
            _board          = std::move(other._board);
            _hand           = std::move(other._hand);
            _blinds         = other._blinds;
            _pot            = other._pot;
            _currentStreet  = other._currentStreet;
            _lastActionTime = other._lastActionTime;
            _players        = other._players;
            _playersStatus  = other._playersStatus;
            _winner         = other._winner;
            _ended          = other._ended;
            _initialized    = other._initialized;
        }

        return *this;
    }

    auto Round::init(const Hand& hand, const Blinds& blinds, int32_t pot, std::array<Player, 3>& players) -> void {
        _hand        = hand;
        _blinds      = blinds;
        _pot         = pot;
        _players     = &players;
        _initialized = true;

        std::for_each(_players->begin(), _players->end(), [this](Player& player) {
            if (!player.isEliminated())
            {
                _getPlayerStatus(player.getNumber()).reset();
            } else {
                _getPlayerStatus(player.getNumber()).inRound = false;
            }
        });

        _lastActionTime = system_clock::now();
    }

    auto Round::call(uint8_t playerNum, int32_t amount) -> void {
        auto& player = _getPlayer(playerNum);

        _actions.at(_currentStreet).emplace_back(CALL, player, _getAndResetLastActionTime(), amount);
        _getPlayerStatus(playerNum).hasCalled(amount, player.getStack());
        _pot += amount;

        _processStreet(CALL);
    }

    auto Round::bet(uint8_t playerNum, int32_t amount) -> void {
        auto& player = _getPlayer(playerNum);

        _actions.at(_currentStreet).emplace_back(BET, player, _getAndResetLastActionTime(), amount);
        _getPlayerStatus(playerNum).hasBet(amount, player.getStack());
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
        if (_winner == nullptr) { throw std::runtime_error("The round's winner has not been set"); }

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
                {"won", _winner->self()},
                {"winner", _winner->getName()}};
    }

    auto Round::_getPlayer(uint8_t playerNum) -> Player& {
        if (playerNum <= 0 || playerNum > 3) { throw std::invalid_argument("The given player number is invalid"); }

        return _players->at(playerNum - 1);
    }

    auto Round::_getPlayerStatus(uint8_t playerNum) -> PlayerStatus& {
        if (playerNum <= 0 || playerNum > 3) { throw std::invalid_argument("The given player number is invalid"); }

        return _playersStatus.at(playerNum - 1);
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
        auto roundPlayersCount = std::count_if(_playersStatus.begin(), _playersStatus.end(), IsInRound);
        auto allPlayersPlayed  = std::all_of(_playersStatus.begin(), _playersStatus.end(), hasPlayed);

        bool currentPlayerCalled = currentPlayerAction == CALL;

        if ((currentPlayerCalled && roundPlayersCount == 2 && allPlayersPlayed)
            || (currentPlayerCalled && roundPlayersCount == 3 && allPlayersPlayed && _lastAction == CALL)
            || std::all_of(_playersStatus.begin(), _playersStatus.end(), hasChecked))
        {
            _endStreet();
        } else {
            _lastAction = currentPlayerAction;
        }
    }

    auto Round::_determineRoundOver() -> void {
        auto winnerFinder = [](const PlayerStatus& playerStatus) { return !playerStatus.inRound; };

        if (std::count_if(_playersStatus.begin(), _playersStatus.end(), winnerFinder) == 1)
        {
            auto roundWinner = std::find_if(_playersStatus.begin(), _playersStatus.end(), winnerFinder);

            _winner = &_getPlayer(roundWinner->playerNum);
            _ended  = true;
        }
    }

    auto Round::_endStreet() -> void {
        switch (_currentStreet)
        {
            case PREFLOP: _currentStreet = FLOP; break;
            case FLOP: _currentStreet = TURN; break;
            case TURN: _currentStreet = RIVER; break;
            case RIVER: _currentStreet = SHOWDOWN; break;
            case SHOWDOWN: break;
        }
    }
}  // namespace GameHandler