#include "Hand.hpp"

#include <algorithm>

Hand::Hand(const Card& firstCard, const Card& secondCard)
    : firstCard(firstCard)
    , secondCard(secondCard)
    , suited(isSuited())
    , broadway(isBroadway())
    , plur(isPlur())
    , connected(isConnected()) {
    if (firstCard == secondCard) {
        throw invalid_hand("The two given cards are the same (" + firstCard.getShortName() + ")");
    }
}

bool Hand::isSuited() { return firstCard.getSuit() == secondCard.getSuit(); }

bool Hand::isBroadway() {
    return std::ranges::find(BROADWAY, firstCard.getRank()) != BROADWAY.end()
        || std::ranges::find(BROADWAY, secondCard.getRank()) != BROADWAY.end();
}

bool Hand::isPlur() {
    return std::ranges::find(BROADWAY, firstCard.getRank()) != BROADWAY.end()
        && std::ranges::find(BROADWAY, secondCard.getRank()) != BROADWAY.end();
}

bool Hand::isConnected() {
    return std::abs(static_cast<int16_t>(firstCard.getRank()) - static_cast<int16_t>(secondCard.getRank())) <= 1
        || firstCard.getRank() == Rank::ACE && secondCard.getRank() == Rank::TWO
        || secondCard.getRank() == Rank::ACE && firstCard.getRank() == Rank::TWO;
}
