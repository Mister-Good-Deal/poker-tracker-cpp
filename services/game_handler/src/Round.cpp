#include "Round.hpp"

namespace GameHandler {
    using std::chrono::duration_cast;
    using std::chrono::seconds;
    using std::ranges::for_each;

    using enum RoundAction::ActionType;
    using enum Round::Street;

    auto Round::operator=(const Round& other) -> Round& {
        if (this != &other)
        {
            _actions        = other._actions;
            _board          = other._board;
            _hand           = other._hand;
            _pot            = other._pot;
            _bet            = other._bet;
            _currentStreet  = other._currentStreet;
            _lastActionTime = other._lastActionTime;
            _winner         = other._winner;
        }

        return *this;
    }

    auto Round::operator=(Round&& other) noexcept -> Round& {
        if (this != &other)
        {
            _actions        = std::move(other._actions);
            _board          = std::move(other._board);
            _hand           = std::move(other._hand);
            _pot            = other._pot;
            _bet            = other._bet;
            _currentStreet  = other._currentStreet;
            _lastActionTime = other._lastActionTime;
            _winner         = other._winner;
        }

        return *this;
    }

    auto Round::endStreet() -> void {
        switch (_currentStreet)
        {
            case PREFLOP: _currentStreet = FLOP; break;
            case FLOP: _currentStreet = TURN; break;
            case TURN: _currentStreet = RIVER; break;
            case RIVER: _currentStreet = SHOWDOWN; break;
            case SHOWDOWN: break;
        }
    }

    auto Round::start() -> void { _lastActionTime = system_clock::now(); }
    auto Round::end(const Player& winner) -> void { _winner = &winner; }

    auto Round::call(const Player& player, int32_t amount) -> void {
        _actions.at(_currentStreet).emplace_back(CALL, player, _getAndResetLastActionTime(), amount);

        if (player.self()) { _bet += amount; }

        _pot += amount;
    }

    auto Round::bet(const Player& player, int32_t amount) -> void {
        _actions.at(_currentStreet).emplace_back(BET, player, _getAndResetLastActionTime(), amount);

        if (player.self()) { _bet += amount; }

        _pot += amount;
    }

    auto Round::check(const Player& player) -> void {
        _actions.at(_currentStreet).emplace_back(CHECK, player, _getAndResetLastActionTime());
    }

    auto Round::fold(const Player& player) -> void {
        _actions.at(_currentStreet).emplace_back(FOLD, player, _getAndResetLastActionTime());
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
                {"pot", _pot},
                {"bet", _bet},
                {"won", _winner->self()},
                {"winner", _winner->getName()}};
    }

    auto Round::_getAndResetLastActionTime() -> seconds {
        auto now            = system_clock::now();
        auto lastActionTime = duration_cast<seconds>(now - _lastActionTime);

        _lastActionTime = now;

        return lastActionTime;
    }
}  // namespace GameHandler