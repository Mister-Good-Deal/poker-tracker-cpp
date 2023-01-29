#include "CardFactory.hpp"

#include <stdexcept>

using Rank = Card::Rank;
using Suit = Card::Suit;

namespace Factory {
    auto CardFactory::create(const std::string& shortCardName) -> Card {
        if (CARD_PROTOTYPES.count(shortCardName) == 0) {
            throw invalid_card("Invalid short card name (" + shortCardName + ")");
        }

        return CARD_PROTOTYPES.at(shortCardName);
    }
}  // namespace Factory
