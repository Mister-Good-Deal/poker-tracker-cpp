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
            _won            = other._won;
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
            _won            = other._won;
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
    auto Round::end(bool isWon) -> void { _won = isWon; }

    auto Round::call(const Player& player, int32_t amount) -> void {
        auto now   = system_clock::now();
        auto index = static_cast<int>(_currentStreet);

        _actions.at(index).emplace_back(CALL, player, duration_cast<seconds>(now - _lastActionTime), amount);

        if (player.self()) { _bet += amount; }

        _pot += amount;
        _lastActionTime = now;
    }

    auto Round::bet(const Player& player, int32_t amount) -> void {
        auto now   = system_clock::now();
        auto index = static_cast<int>(_currentStreet);

        _actions.at(index).emplace_back(BET, player, duration_cast<seconds>(now - _lastActionTime), amount);

        if (player.self()) { _bet += amount; }

        _pot += amount;
        _lastActionTime = now;
    }

    auto Round::check(const Player& player) -> void {
        auto now   = system_clock::now();
        auto index = static_cast<int>(_currentStreet);

        _actions.at(index).emplace_back(CHECK, player, duration_cast<seconds>(now - _lastActionTime));

        _lastActionTime = now;
    }

    auto Round::fold(const Player& player) -> void {
        auto now   = system_clock::now();
        auto index = static_cast<int>(_currentStreet);

        _actions.at(index).emplace_back(FOLD, player, duration_cast<seconds>(now - _lastActionTime));

        _lastActionTime = now;
    }

    auto Round::toJson() const -> json {
        auto preFlopActions = json::array();
        auto flopActions    = json::array();
        auto turnActions    = json::array();
        auto riverActions   = json::array();

        for_each(_actions[static_cast<int8_t>(PREFLOP)], [&](const auto& action) { preFlopActions.emplace_back(action.toJson()); });
        for_each(_actions[static_cast<int8_t>(FLOP)], [&](const RoundAction& action) { flopActions.emplace_back(action.toJson()); });
        for_each(_actions[static_cast<int8_t>(TURN)], [&](const RoundAction& action) { turnActions.emplace_back(action.toJson()); });
        for_each(_actions[static_cast<int8_t>(RIVER)], [&](const RoundAction& action) { riverActions.emplace_back(action.toJson()); });

        return {{"actions", {{"pre_flop", preFlopActions}, {"flop", flopActions}, {"turn", turnActions}, {"river", riverActions}}},
                {"board", _board.toJson()},
                {"hand", _hand.toJson()},
                {"pot", _pot},
                {"bet", _bet},
                {"won", _won}};
    }
}  // namespace GameHandler