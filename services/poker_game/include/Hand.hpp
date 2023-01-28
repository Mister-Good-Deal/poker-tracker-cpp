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

        std::tuple<Card, Card> cards() { return {firstCard, secondCard}; }

    protected:
        virtual bool isSuited();
        virtual bool isBroadway();
        virtual bool isPlur();
        virtual bool isConnected();

    private:
        Card firstCard;
        Card secondCard;
        bool suited;
        bool broadway;
        bool plur;
        bool connected;
};
