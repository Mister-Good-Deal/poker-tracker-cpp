#include "RoundAction.hpp"

namespace GameHandler {
    RoundAction::RoundAction(RoundAction::ActionType action, Player player, seconds time, int32_t amount) :
        action(action), player(std::move(player)), time(time), amount(amount) {}
}  // namespace GameHandler
