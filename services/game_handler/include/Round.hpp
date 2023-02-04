#pragma once

#include <array>
#include <chrono>
#include <map>
#include <string>
#include <vector>

#include "Board.hpp"
#include "RoundAction.hpp"

namespace GameHandler {
    using std::chrono::system_clock;
    using std::chrono::time_point;

    constexpr int8_t STREET_NUMBER = 5;

    class UnknownPlayerException : public std::runtime_error {
        public:
            explicit UnknownPlayerException(const std::string& arg) : runtime_error(arg) {}
    };

    class Round {
        public:
            using round_actions_t = std::array<std::vector<RoundAction>, STREET_NUMBER>;

            enum class Street : int8_t { PREFLOP = 0, FLOP, TURN, RIVER, SHOWDOWN };

            Round()                   = default;
            Round(const Round& other) = default;
            Round(Round&& other) noexcept { *this = std::move(other); };

            virtual ~Round() = default;

            auto operator=(const Round& other) -> Round&;
            auto operator=(Round&& other) noexcept -> Round&;

            [[nodiscard]] auto getBoard() const -> const Board& { return board; }
            [[nodiscard]] auto getPot() const -> int32_t { return pot; }

            auto start() -> void;
            auto endStreet() -> void;
            auto end() -> void;

            auto call(const Player& player, int32_t amount) -> void;
            auto bet(const Player& player, int32_t amount) -> void;
            auto check(const Player& player) -> void;
            auto fold(const Player& player) -> void;

        private:
            round_actions_t          actions;
            Board                    board;
            int32_t                  pot            = 0;
            Street                   currentStreet  = Street::PREFLOP;
            time_point<system_clock> lastActionTime = system_clock::now();
    };
}  // namespace GameHandler
