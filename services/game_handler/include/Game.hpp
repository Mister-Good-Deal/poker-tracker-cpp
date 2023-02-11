#pragma once

#include "Round.hpp"

namespace GameHandler {
    using std::chrono::system_clock;
    using std::chrono::time_point;

    class Game {
        public:
            Game()                  = default;
            Game(const Game& other) = default;
            Game(Game&& other) noexcept { *this = std::move(other); };

            virtual ~Game() = default;

            auto operator=(const Game& other) -> Game&;
            auto operator=(Game&& other) noexcept -> Game&;

            auto getCurrentRound() -> Round& { return _rounds.back(); };

            auto setBuyIn(int32_t buyIn) -> void { _buyIn = buyIn; }
            auto setMultipliers(int32_t multipliers) -> void { _multipliers = multipliers; }
            auto setWinner(const Player& winner) -> void { _winner = &winner; }

            auto init(const Player& player1, const Player& player2, const Player& player3) -> void;
            auto newRound() -> void;
            auto getPlayer(const std::string& playerName) -> Player;
            auto end() -> void;

            [[nodiscard]] auto toJson() const -> json;

        private:
            std::vector<Round>            _rounds;
            std::map<std::string, Player> _players;
            time_point<system_clock>      _startTime;
            time_point<system_clock>      _endTime;
            const Player*                 _winner      = nullptr;
            int32_t                       _buyIn       = 0;
            int32_t                       _multipliers = 2;

            [[nodiscard]] auto _computeBalance() const -> int32_t;
    };
}  // namespace GameHandler