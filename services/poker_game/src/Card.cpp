#include "Card.hpp"

using Rank = Card::Rank;
using Suit = Card::Suit;

Card::Card(Card&& other) noexcept : rank(std::move(other.rank)), suit(std::move(other.suit)) {
    other.rank = Rank::UNKNOWN;
    other.suit = Suit::UNKNOWN;
}

auto Card::operator=(Card other) -> Card& {
    std::swap(rank, other.rank);
    std::swap(suit, other.suit);

    return *this;
}

auto Card::operator=(Card&& other) -> Card& {
    rank = std::move(other.rank);
    suit = std::move(other.suit);

    return *this;
}

auto Card::swap(Card& other) -> void {
    std::swap(rank, other.rank);
    std::swap(suit, other.suit);
}

auto Card::getFullName() const -> std::string { return rankToString(rank) + " of " + suitToString(suit); }
auto Card::getShortName() const -> std::string { return rankToShortString(rank) + suitToString(suit)[0]; }
