#include "CardFactory.hpp"

#include <stdexcept>

namespace GameHandler::Factory {
    auto CardFactory::create(const std::string& shortCardName) -> Card {
        if (!CARD_PROTOTYPES.contains(shortCardName)) {
            throw invalid_card("Invalid short card name (" + shortCardName + ")");
        }

        return CARD_PROTOTYPES.at(shortCardName);
    }
}  // namespace GameHandler::Factory
