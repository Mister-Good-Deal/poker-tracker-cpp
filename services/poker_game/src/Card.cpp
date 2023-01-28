#include "Card.hpp"

using Rank = Card::Rank;
using Suit = Card::Suit;

Card::Card(Rank rank, Suit suit) : rank(rank), suit(suit) {}

std::string Card::getFullName() { return rankToString(rank) + " of " + suitToString(suit); }
std::string Card::getShortName() { return rankToShortString(rank) + suitToString(suit)[0]; }
