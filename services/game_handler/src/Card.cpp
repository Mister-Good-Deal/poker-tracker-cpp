#include "Card.hpp"

namespace GameHandler {
    using Rank = Card::Rank;
    using Suit = Card::Suit;

    auto Card::operator=(Card&& other) noexcept -> Card& {
        rank = other.rank;
        suit = other.suit;

        return *this;
    }

    auto Card::getFullName() const -> std::string { return rankToString(rank) + " of " + suitToString(suit); }
    auto Card::getShortName() const -> std::string { return rankToShortString(rank) + suitToString(suit)[0]; }
}  // namespace GameHandler