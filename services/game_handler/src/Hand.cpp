#include "Hand.hpp"

#include <algorithm>

namespace GameHandler {
    using ::std::tie;
    using ::std::ranges::find;

    Hand::Hand(const Card& firstCard, const Card& secondCard) : firstCard(firstCard), secondCard(secondCard) {
        if (firstCard == secondCard) {
            throw invalid_hand("The two given cards are the same (" + firstCard.getShortName() + ")");
        }

        processHand();
    }

    auto Hand::operator=(const Hand& other) -> Hand& {
        if (this != &other) {
            firstCard  = other.firstCard;
            secondCard = other.secondCard;
            suited     = other.suited;
            broadway   = other.broadway;
            plur       = other.plur;
            connected  = other.connected;
        }

        return *this;
    }

    auto Hand::operator=(Hand&& other) noexcept -> Hand& {
        if (this != &other) {
            firstCard  = std::move(other.firstCard);
            secondCard = std::move(other.secondCard);
            suited     = other.suited;
            broadway   = other.broadway;
            plur       = other.plur;
            connected  = other.connected;
        }

        return *this;
    }

    auto Hand::operator==(const Hand& rhs) const -> bool { return tie(firstCard, secondCard) == tie(rhs.firstCard, rhs.secondCard); }

    auto Hand::operator!=(const Hand& rhs) const -> bool { return !(rhs == *this); }

    auto Hand::isSuited() -> bool { return firstCard.getSuit() == secondCard.getSuit(); }

    auto Hand::isBroadway() -> bool {
        return find(BROADWAY, firstCard.getRank()) != BROADWAY.end() || find(BROADWAY, secondCard.getRank()) != BROADWAY.end();
    }

    auto Hand::isPlur() -> bool {
        return find(BROADWAY, firstCard.getRank()) != BROADWAY.end() && find(BROADWAY, secondCard.getRank()) != BROADWAY.end();
    }

    auto Hand::isConnected() -> bool {
        return std::abs(static_cast<int16_t>(firstCard.getRank()) - static_cast<int16_t>(secondCard.getRank())) <= 1
            || (firstCard.getRank() == Rank::ACE && secondCard.getRank() == Rank::TWO)
            || (secondCard.getRank() == Rank::ACE && firstCard.getRank() == Rank::TWO);
    }

    auto Hand::processHand() -> void {
        suited    = isSuited();
        broadway  = isBroadway();
        plur      = isPlur();
        connected = isConnected();
    }
}  // namespace GameHandler