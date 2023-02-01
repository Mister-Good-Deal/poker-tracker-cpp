#pragma once

#include <string>

#include "Round.hpp"

namespace GameHandler {
    class Game {
        public:
            Game(const Game& other) = default;
            Game(Game&& other) noexcept : currentRound(rounds.back()) { *this = std::move(other); };

            virtual ~Game() = default;

            auto operator=(const Game& other) -> Game&;
            auto operator=(Game&& other) noexcept -> Game&;

            auto getCurrentRound() -> Round& { return currentRound; };

            auto initGame(const Player& player1, const Player& player2, const Player& player3) -> void;
            auto newRound() -> void;
            auto getPlayer(const std::string& playerName) -> Player;

        private:
            std::vector<Round>             rounds;
            Round&                         currentRound;
            std::map<std::string, Player&> players;
    };
}  // namespace GameHandler