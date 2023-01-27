#pragma once

#include <string>

class Card {
    public:
        enum class Suit {
            HEART,
            DIAMOND,
            CLUB,
            SPADE
        };

        enum class Rank {
            TWO,
            THREE,
            FOUR,
            FIVE,
            SIX,
            SEVEN,
            EIGHT,
            NINE,
            TEN,
            JACK,
            QUEEN,
            KING,
            ACE
        };

    public:
        Card(Rank rank, Suit suit);

        std::string getFullName();
        std::string getShortName();

    private:
        Rank rank;
        Suit suit;
};
