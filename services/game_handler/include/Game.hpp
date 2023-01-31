#pragma once

#include <string>

class Player;
class Round;

namespace GameHandler {
    class Game {
        public:
            auto initGame(const Player& player1, const Player& player2, const Player& player3) -> void;
            auto newRound() -> void;
            auto getPlayer(const std::string& playerName) -> Player;
            auto getCurrentRound() -> Round;
    };
}  // namespace GameHandler