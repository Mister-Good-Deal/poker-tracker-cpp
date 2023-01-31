#include "Hand.hpp"

#include <algorithm>

namespace GameHandler {
    using std::abs;
    using std::move;
    using std::swap;
    using std::tie;
    using std::ranges::find;

    namespace std {
        void swap(Card& lhs, Card& rhs) { lhs.swap(rhs); }
    }  // namespace std

    Hand::Hand(Hand&& other) noexcept :
        firstCard(move(other.firstCard)), secondCard(move(other.secondCard)), suited(other.suited), broadway(other.broadway),
        plur(other.plur), connected(other.connected) {
        auto emptyFirstCard  = Card();
        auto emptySecondCard = Card();

        other.firstCard  = emptyFirstCard;
        other.secondCard = emptySecondCard;
        other.suited     = false;
        other.broadway   = false;
        other.plur       = false;
        other.connected  = false;
    }

    Hand::Hand(const Card& firstCard, const Card& secondCard) : firstCard(firstCard), secondCard(secondCard) {
        if (firstCard == secondCard) {
            throw invalid_hand("The two given cards are the same (" + firstCard.getShortName() + ")");
        }

        processHand();
    }

    auto Hand::operator==(const Hand& rhs) const -> bool { return tie(firstCard, secondCard) == tie(rhs.firstCard, rhs.secondCard); }

    auto Hand::operator!=(const Hand& rhs) const -> bool { return !(rhs == *this); }

    auto Hand::operator=(Hand other) -> Hand& {
        if (this != &other) {
            std::swap(firstCard, other.firstCard);
            std::swap(secondCard, other.secondCard);
            swap(suited, other.suited);
            swap(broadway, other.broadway);
            swap(plur, other.plur);
            swap(connected, other.connected);
        }

        return *this;
    }

    auto Hand::isSuited() -> bool { return firstCard.getSuit() == secondCard.getSuit(); }

    auto Hand::isBroadway() -> bool {
        return find(BROADWAY, firstCard.getRank()) != BROADWAY.end() || find(BROADWAY, secondCard.getRank()) != BROADWAY.end();
    }

    auto Hand::isPlur() -> bool {
        return find(BROADWAY, firstCard.getRank()) != BROADWAY.end() && find(BROADWAY, secondCard.getRank()) != BROADWAY.end();
    }

    auto Hand::isConnected() -> bool {
        return abs(static_cast<int16_t>(firstCard.getRank()) - static_cast<int16_t>(secondCard.getRank())) <= 1
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