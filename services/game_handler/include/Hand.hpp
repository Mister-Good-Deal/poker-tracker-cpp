#pragma once

#include <vector>

#include "Card.hpp"

namespace GameHandler {
    using Rank = Card::Rank;

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

            class invalid_hand : public std::runtime_error {
                public:
                    explicit invalid_hand(const std::string& arg) : runtime_error(arg){};
            };

            auto cards() -> std::tuple<Card, Card> { return {firstCard, secondCard}; }

        protected:
            auto isSuited() -> bool;
            auto isBroadway() -> bool;
            auto isPlur() -> bool;
            auto isConnected() -> bool;

        private:
            Card firstCard;
            Card secondCard;
            bool suited    = false;
            bool broadway  = false;
            bool plur      = false;
            bool connected = false;

            auto processHand() -> void;

            const std::vector<Rank> BROADWAY = {Rank::TEN, Rank::JACK, Rank::QUEEN, Rank::KING, Rank::ACE};
    };
}  // namespace GameHandler