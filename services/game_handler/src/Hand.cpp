#include "Hand.hpp"

#include <algorithm>

namespace GameHandler {
    using ::std::tie;
    using ::std::ranges::all_of;
    using ::std::ranges::any_of;

    Hand::Hand(const Card& firstCard, const Card& secondCard) :
        firstCard(firstCard), secondCard(secondCard), cards({&firstCard, &secondCard}) {
        if (firstCard == secondCard) {
            throw invalid_hand("The two given cards are the same (" + firstCard.getShortName() + ")");
        }

        processHand();
    }

    auto Hand::operator=(const Hand& other) -> Hand& {
        if (this != &other) {
            firstCard  = other.firstCard;
            secondCard = other.secondCard;
            cards      = other.cards;
            suited     = other.suited;
            broadway   = other.broadway;
            plur       = other.plur;
            connected  = other.connected;
            premium    = other.premium;
        }

        return *this;
    }

    auto Hand::operator=(Hand&& other) noexcept -> Hand& {
        if (this != &other) {
            firstCard  = std::move(other.firstCard);
            secondCard = std::move(other.secondCard);
            cards      = std::move(other.cards);
            suited     = other.suited;
            broadway   = other.broadway;
            plur       = other.plur;
            connected  = other.connected;
            premium    = other.premium;
        }

        return *this;
    }

    auto Hand::operator==(const Hand& rhs) const -> bool { return tie(firstCard, secondCard) == tie(rhs.firstCard, rhs.secondCard); }
    auto Hand::operator!=(const Hand& rhs) const -> bool { return !(rhs == *this); }

    auto Hand::isSuited() -> bool { return firstCard.getSuit() == secondCard.getSuit(); }

    auto Hand::isAceSuited() -> bool {
        return isSuited() && any_of(cards, [](const auto& card) { return card->getRank() == Rank::ACE; });
    }

    auto Hand::isBroadway() -> bool {
        return any_of(cards, [this](const auto& card) { return isBroadway(card); });
    }

    auto Hand::isPlur() -> bool {
        return all_of(cards, [this](const auto& card) { return isBroadway(card); });
    }

    auto Hand::isConnected() -> bool {
        return std::abs(static_cast<int16_t>(firstCard.getRank()) - static_cast<int16_t>(secondCard.getRank())) <= 1
            || (firstCard.getRank() == Rank::ACE && secondCard.getRank() == Rank::TWO)
            || (secondCard.getRank() == Rank::ACE && firstCard.getRank() == Rank::TWO);
    }

    auto Hand::isPremium() -> bool {
        return find(PREMIUM, std::array<Rank, 2>{firstCard.getRank(), secondCard.getRank()}) != PREMIUM.end();
    }

    auto Hand::processHand() -> void {
        suited    = isSuited();
        aceSuited = isAceSuited();
        broadway  = isBroadway();
        plur      = isPlur();
        connected = isConnected();
        premium   = isPremium();
    }

    auto Hand::toJson() -> json {
        auto cardsArray = json::array();

        cardsArray.emplace_back(firstCard.toJson());
        cardsArray.emplace_back(secondCard.toJson());

        return {{"cards", cardsArray},
                {"qualifiers",
                 {{"suited", suited},
                  {"aceSuited", aceSuited},
                  {"broadway", broadway},
                  {"plur", plur},
                  {"connected", connected},
                  {"premium", premium}}}};
    }
}  // namespace GameHandler