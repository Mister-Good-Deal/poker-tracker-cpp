#include "RoundAction.hpp"

namespace GameHandler {
    using enum RoundAction::ActionType;

    auto RoundAction::operator=(RoundAction&& other) noexcept -> RoundAction& {
        if (this != &other)
        {
            _action = other._action;
            _player = std::move(other._player);
            _time   = other._time;
            _amount = other._amount;
        }

        return *this;
    }

    auto RoundAction::toJson() const -> json {
        json object = {{"action", actionToString(_action)}, {"player_name", _player.getName()}, {"elapsed_time", _time.count()}};

        if (_action == CALL || _action == BET) { object["amount"] = _amount; }

        return object;
    }
}  // namespace GameHandler
