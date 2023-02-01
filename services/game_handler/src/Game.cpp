#include "Game.hpp"

namespace GameHandler {
    auto Game::operator=(Game&& other) noexcept -> Game& {
        if (this != &other) {
            rounds       = std::move(other.rounds);
            players      = std::move(other.players);
            currentRound = rounds.back();
        }

        return *this;
    }

    auto Game::operator=(const Game& other) -> Game& {
        if (this != &other) {
            rounds       = other.rounds;
            players      = other.players;
            currentRound = rounds.back();
        }

        return *this;
    }

    auto Game::initGame(const Player& player1, const Player& player2, const Player& player3) -> void {
        players.at(player1.getName()) = player1;
        players.at(player2.getName()) = player2;
        players.at(player3.getName()) = player3;
    }

    auto Game::newRound() -> void {
        rounds.emplace_back();

        currentRound = rounds.back();
    }

    auto Game::getPlayer(const std::string& playerName) -> Player {
        if (players.find(playerName) == players.end()) {
            throw UnknownPlayerException("The player " + playerName + " is unknown in this game");
        }

        return players.at(playerName);
    }

}  // namespace GameHandler