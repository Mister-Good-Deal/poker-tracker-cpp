#pragma once

#include <cstdint>

#include <chrono>

#include "Player.hpp"

namespace GameHandler {
    using std::chrono::seconds;

    class RoundAction {
        public:
            enum class ActionType : int8_t { CHECK = 0, CALL, BET, FOLD };

            RoundAction(ActionType action, Player player, seconds time, int32_t amount = 0);

        private:
            ActionType action;
            Player     player;
            seconds    time;
            int32_t    amount = 0;
    };
}  // namespace GameHandler
