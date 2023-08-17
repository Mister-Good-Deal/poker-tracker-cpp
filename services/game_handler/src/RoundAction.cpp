#include "game_handler/RoundAction.hpp"

namespace GameHandler {
    using enum RoundAction::ActionType;

    auto RoundAction::operator=(RoundAction&& other) noexcept -> RoundAction& {
        if (this != &other) {
            _action = other._action;
            _player = std::move(other._player);
            _time   = other._time;
            _amount = other._amount;
        }

        return *this;
    }

    auto RoundAction::toJson() const -> json {
        json object = {{"action", fmt::format("{}", _action)},
                       {"player", fmt::format("player_{}", _player.getNumber())},
                       {"elapsed_time", _time.count()}};

        if (_requiresAmount(_action)) { object["amount"] = _amount; }

        return object;
    }

    auto RoundAction::_requiresAmount(ActionType action) const -> bool {
        return action == CALL || action == BET || action == RAISE || action == PAY_BIG_BLIND || action == PAY_SMALL_BLIND;
    }

}  // namespace GameHandler