#pragma once

#include <string>
#include <tuple>

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
        Card() = default;
        Card(Rank rank, Suit suit);

        bool operator==(const Card& rhs) const { return std::tie(rank, suit) == std::tie(rhs.rank, rhs.suit); }
        bool operator!=(const Card& rhs) const { return !(rhs == *this); }

        [[nodiscard]] Rank getRank() const { return rank; }
        [[nodiscard]] Suit getSuit() const { return suit; }

        std::string getFullName();
        std::string getShortName();

    private:
        Rank rank;
        Suit suit;
};
