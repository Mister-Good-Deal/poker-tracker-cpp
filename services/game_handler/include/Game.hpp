#pragma once

#include <string>

class Player;
class Round;

namespace GameHandler {
    class Game {
        public:
            void   initGame(const Player& player1, const Player& player2, const Player& player3);
            void   newRound();
            Player getPlayer(const std::string& playerName);
            Round  getCurrentRound();
    };
}  // namespace GameHandler