#pragma once

#include <vector>

#include "Card.hpp"

using Rank = Card::Rank;

class Hand {
    public:
        Hand(const Card& firstCard, const Card& secondCard);

        const std::vector<Rank> BROADWAY = {Rank::TEN, Rank::JACK, Rank::QUEEN, Rank::KING, Rank::ACE};

        class invalid_hand : public std::runtime_error {
            public:
                explicit invalid_hand(const std::string& arg) : runtime_error(arg){};
        };

        auto cards() -> std::tuple<Card, Card> { return {firstCard, secondCard}; }

    protected:
        virtual auto isSuited() -> bool;
        virtual auto isBroadway() -> bool;
        virtual auto isPlur() -> bool;
        virtual auto isConnected() -> bool;

    private:
        Card firstCard;
        Card secondCard;
        bool suited;
        bool broadway;
        bool plur;
        bool connected;
};
