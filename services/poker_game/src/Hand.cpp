#include "Hand.hpp"

#include <algorithm>

namespace std {
    void swap(Card& lhs, Card& rhs) { lhs.swap(rhs); }
}  // namespace std

Hand::Hand(Hand&& other) noexcept
    : firstCard(std::move(other.firstCard))
    , secondCard(std::move(other.secondCard))
    , suited(other.suited)
    , broadway(other.broadway)
    , plur(other.plur)
    , connected(other.connected) {
    auto emptyFirstCard  = Card();
    auto emptySecondCard = Card();

    other.firstCard  = emptyFirstCard;
    other.secondCard = emptySecondCard;
    other.suited     = false;
    other.broadway   = false;
    other.plur       = false;
    other.connected  = false;
}

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

auto Hand::operator==(const Hand& rhs) const -> bool {
    return std::tie(firstCard, secondCard) == std::tie(rhs.firstCard, rhs.secondCard);
}

auto Hand::operator!=(const Hand& rhs) const -> bool { return !(rhs == *this); }

auto Hand::operator=(Hand other) -> Hand& {
    if (this != &other) {
        std::swap(firstCard, other.firstCard);
        std::swap(secondCard, other.secondCard);
        std::swap(suited, other.suited);
        std::swap(broadway, other.broadway);
        std::swap(plur, other.plur);
        std::swap(connected, other.connected);
    }

    return *this;
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
