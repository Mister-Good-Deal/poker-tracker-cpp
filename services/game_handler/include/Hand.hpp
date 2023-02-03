#pragma once

#include "Card.hpp"

namespace GameHandler {
    using std::ranges::find;

    using Rank = Card::Rank;

    static constexpr std::array<Rank, 5> BROADWAY = {Rank::TEN, Rank::JACK, Rank::QUEEN, Rank::KING, Rank::ACE};

    static constexpr std::array<std::array<Rank, 2>, 5> PREMIUM = {{{{Rank::QUEEN, Rank::QUEEN}},
                                                                    {{Rank::KING, Rank::ACE}},
                                                                    {{Rank::ACE, Rank::KING}},
                                                                    {{Rank::KING, Rank::KING}},
                                                                    {{Rank::ACE, Rank::ACE}}}};

    class invalid_hand : public std::runtime_error {
        public:
            explicit invalid_hand(const std::string& arg) : runtime_error(arg){};
    };

    class Hand {
        public:
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
            Card                       firstCard;
            Card                       secondCard;
            std::array<const Card*, 2> cards;  // Shortcut to use std algorithms in class, ref on the 2 cards
            bool                       suited    = false;
            bool                       aceSuited = false;
            bool                       broadway  = false;
            bool                       plur      = false;
            bool                       connected = false;
            bool                       premium   = false;

            auto processHand() -> void;
            auto isBroadway(const Card* card) -> bool { return find(BROADWAY, card->getRank()) != BROADWAY.end(); }
    };
}  // namespace GameHandler