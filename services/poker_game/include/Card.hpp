#pragma once

#include <iostream>
#include <string>
#include <tuple>

class Card {
    public:
        enum class Rank : int16_t {
            TWO = 0,
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

        enum class Suit : int16_t {
            HEART = 0,
            DIAMOND,
            CLUB,
            SPADE
        };

        static constexpr std::string rankToString(Rank rank) {
            switch (rank) {
                case Rank::TWO: return "Two";
                case Rank::THREE: return "Three";
                case Rank::FOUR: return "Four";
                case Rank::FIVE: return "Five";
                case Rank::SIX: return "Six";
                case Rank::SEVEN: return "Seven";
                case Rank::EIGHT: return "Eight";
                case Rank::NINE: return "Nine";
                case Rank::TEN: return "Ten";
                case Rank::JACK: return "Jack";
                case Rank::QUEEN: return "Queen";
                case Rank::KING: return "King";
                case Rank::ACE: return "Ace";
                default: throw std::invalid_argument("The given rank is invalid");
            }
        }

        static constexpr std::string rankToShortString(Rank rank) {
            switch (rank) {
                case Rank::TWO: return "2";
                case Rank::THREE: return "3";
                case Rank::FOUR: return "4";
                case Rank::FIVE: return "5";
                case Rank::SIX: return "6";
                case Rank::SEVEN: return "7";
                case Rank::EIGHT: return "8";
                case Rank::NINE: return "9";
                case Rank::TEN: return "T";
                case Rank::JACK: return "J";
                case Rank::QUEEN: return "Q";
                case Rank::KING: return "K";
                case Rank::ACE: return "A";
                default: throw std::invalid_argument("The given rank is invalid");
            }
        }

        static constexpr std::string suitToString(Suit suit) {
            switch (suit) {
                case Suit::HEART: return "Heart";
                case Suit::DIAMOND: return "Diamond";
                case Suit::CLUB: return "Club";
                case Suit::SPADE: return "Spade";
                default: throw std::invalid_argument("The given suit is invalid");
            }
        }

    public:
        Card() = default;
        Card(Rank rank, Suit suit);

        bool operator==(const Card& rhs) const { return std::tie(rank, suit) == std::tie(rhs.rank, rhs.suit); }
        bool operator!=(const Card& rhs) const { return !(rhs == *this); }

        [[nodiscard]] Rank getRank() const { return rank; }
        [[nodiscard]] Suit getSuit() const { return suit; }

        [[nodiscard]] std::string getFullName() const;
        [[nodiscard]] std::string getShortName() const;

    private:
        Rank rank;
        Suit suit;
};
