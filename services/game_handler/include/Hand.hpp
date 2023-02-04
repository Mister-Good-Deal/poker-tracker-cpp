#pragma once

#include "Card.hpp"

namespace GameHandler {
    using std::ranges::find;

    using enum Card::Rank;

    constexpr int8_t BROADWAY_NUMBER   = 5;
    constexpr int8_t PREMIUM_NUMBER    = 5;
    constexpr int8_t HAND_CARDS_NUMBER = 2;

    static constexpr std::array<Card::Rank, BROADWAY_NUMBER> BROADWAY = {TEN, JACK, QUEEN, KING, ACE};

    static constexpr std::array<std::array<Card::Rank, HAND_CARDS_NUMBER>, PREMIUM_NUMBER> PREMIUM = {
        {{{QUEEN, QUEEN}}, {{KING, ACE}}, {{ACE, KING}}, {{KING, KING}}, {{ACE, ACE}}}};

    class invalid_hand : public std::runtime_error {
        public:
            explicit invalid_hand(const std::string& arg) : runtime_error(arg){};
    };

    class Hand {
        public:
            typedef std::array<const Card*, HAND_CARDS_NUMBER> hand_cards_t;

            Hand()                  = default;
            Hand(const Hand& other) = default;
            Hand(Hand&& other) noexcept { *this = std::move(other); };
            Hand(const Card& firstCard, const Card& secondCard);

            virtual ~Hand() = default;

            auto operator=(const Hand& other) -> Hand&;
            auto operator=(Hand&& other) noexcept -> Hand&;

            auto operator==(const Hand& rhs) const -> bool;
            auto operator!=(const Hand& rhs) const -> bool;

            auto toJson() -> json;

        protected:
            auto isSuited() -> bool;
            auto isAceSuited() -> bool;
            auto isBroadway() -> bool;
            auto isPlur() -> bool;
            auto isConnected() -> bool;
            auto isPremium() -> bool;

        private:
            Card         firstCard;
            Card         secondCard;
            hand_cards_t cards     = {&firstCard, &secondCard};  // Shortcut to use std algorithms in class
            bool         suited    = false;
            bool         aceSuited = false;
            bool         broadway  = false;
            bool         plur      = false;
            bool         connected = false;
            // @todo sibblingsConnected
            bool premium = false;

            auto processHand() -> void;
            auto isBroadway(const Card* card) -> bool { return find(BROADWAY, card->getRank()) != BROADWAY.end(); }
    };
}  // namespace GameHandler