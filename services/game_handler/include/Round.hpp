#pragma once

#include <array>
#include <chrono>
#include <map>
#include <string>
#include <vector>

#include "RoundAction.hpp"

namespace GameHandler {
    using std::chrono::system_clock;
    using std::chrono::time_point;

    class Round {
        public:
            enum class Street : int8_t { PREFLOP = 0, FLOP, TURN, RIVER, SHOWDOWN };

            Round() = default;

            [[nodiscard]] auto getPot() const -> int32_t { return pot; }

            auto start() -> void;
            auto endStreet() -> void;
            auto end() -> void;

            auto call(const Player& player, int32_t amount) -> void;
            auto bet(const Player& player, int32_t amount) -> void;
            auto check(const Player& player) -> void;
            auto fold(const Player& player) -> void;

        private:
            std::array<std::vector<RoundAction>, 5> actions;
            std::map<Player*, int32_t>              playersBets;
            int32_t                                 pot            = 0;
            Street                                  currentStreet  = Street::PREFLOP;
            int32_t                                 callAmount     = 0;
            time_point<system_clock>                lastActionTime = system_clock::now();
    };
}  // namespace GameHandler
