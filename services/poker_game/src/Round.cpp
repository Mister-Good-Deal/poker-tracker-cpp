#include "Round.hpp"

#include "Player.hpp"

/**
 * @brief player call the current call amount
 * @param player : the player who calls
 */
void Round::call(Player& player) {
    int32_t previousBet = 0;

    if (playersBets.find(&player) != playersBets.end()) {
        previousBet = playersBets[&player];
    }

    int32_t betAmount = callAmount - previousBet;
    playersBets[&player] += betAmount;
    pot += betAmount;

    playersActions.at(static_cast<int>(currentStreet)).push_back(player.getName() + " called " + std::to_string(betAmount) + " chips");
}

/**
 * @brief player bet a certain amount
 * @param player : the player who bet
 * @param amount : the amount bet
 */
void Round::bet(Player& player, int32_t amount) {
    int32_t previousBet = 0;
    if (playersBets.find(&player) != playersBets.end()) {
        previousBet = playersBets[&player];
    }
    int32_t betAmount = amount;
    playersBets[&player] += betAmount;
    callAmount = playersBets[&player];
    pot += betAmount;
    playersActions.at(static_cast<int>(currentStreet)).push_back(player.getName() + " bet " + std::to_string(betAmount) + " chips");
}

/**
 * @brief player check
 * @param player : the player who check
 */
void Round::check(Player& player) {
    if (playersBets.find(&player) == playersBets.end()) {
        playersBets[&player] = 0;
    }
    playersActions.at(static_cast<int>(currentStreet)).push_back(player.getName() + " checked");
}

/**
 * @brief player fold
 * @param player : the player who fold
 */
void Round::fold(Player& player) {
    playersBets.erase(&player);
    playersActions.at(static_cast<int>(currentStreet)).push_back(player.getName() + " folded");
}
