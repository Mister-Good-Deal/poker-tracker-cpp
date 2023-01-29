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

auto Hand::isSuited() -> bool { return firstCard.getSuit() == secondCard.getSuit(); }

auto Hand::isBroadway() -> bool {
    return std::ranges::find(BROADWAY, firstCard.getRank()) != BROADWAY.end()
        || std::ranges::find(BROADWAY, secondCard.getRank()) != BROADWAY.end();
}

auto Hand::isPlur() -> bool {
    return std::ranges::find(BROADWAY, firstCard.getRank()) != BROADWAY.end()
        && std::ranges::find(BROADWAY, secondCard.getRank()) != BROADWAY.end();
}

auto Hand::isConnected() -> bool {
    return std::abs(static_cast<int16_t>(firstCard.getRank()) - static_cast<int16_t>(secondCard.getRank())) <= 1
        || firstCard.getRank() == Rank::ACE && secondCard.getRank() == Rank::TWO
        || secondCard.getRank() == Rank::ACE && firstCard.getRank() == Rank::TWO;
}
