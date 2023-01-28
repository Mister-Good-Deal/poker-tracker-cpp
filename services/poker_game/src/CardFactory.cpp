#include "CardFactory.hpp"

#include <stdexcept>

using Rank = Card::Rank;
using Suit = Card::Suit;

namespace Factory {
    Card CardFactory::create(const std::string& shortCardName) {
        if (CARD_PROTOTYPES.count(shortCardName) == 0) {
            throw std::invalid_argument("Invalid short card name: " + shortCardName);
        }

        return CARD_PROTOTYPES[shortCardName];
    }
}
