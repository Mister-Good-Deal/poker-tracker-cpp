#include "game_handler/RoundAction.hpp"

namespace GameHandler {
    using enum RoundAction::ActionType;

    RoundAction::RoundAction(RoundAction::ActionType action, Player player, seconds time, int32_t amount)
      : _action(action)
      , _player(std::move(player))
      , _time(time)
      , _amount(amount) {}

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
        return action == CALL || action == BET || action == RAISE || action == ALL_IN || action == PAY_BIG_BLIND || action == PAY_SMALL_BLIND;
    }
}  // namespace GameHandler