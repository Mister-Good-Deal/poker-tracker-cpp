#include "Card.hpp"

using Rank = Card::Rank;
using Suit = Card::Suit;

Card::Card(Rank rank, Suit suit) : rank(rank), suit(suit) {}

auto Card::getFullName() const -> std::string { return rankToString(rank) + " of " + suitToString(suit); }
auto Card::getShortName() const -> std::string { return rankToShortString(rank) + suitToString(suit)[0]; }
