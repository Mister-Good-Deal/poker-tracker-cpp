#pragma once

#include <iostream>
#include <string>
#include <tuple>

#include <nlohmann/json.hpp>

namespace GameHandler {
    using json = nlohmann::json;

    constexpr int8_t RANK_CARDS_NUMBER = 13;
    constexpr int8_t SUIT_CARDS_NUMBER = 4;

    class Card {
        public:
            enum class Rank : int8_t { TWO = 1, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE, UNDEFINED };

            enum class Suit : int8_t { HEART = 0, DIAMOND, CLUB, SPADE, UNKNOWN };

            static constexpr auto rankToString(Rank rank) -> std::string {
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

            static constexpr auto rankToShortString(Rank rank) -> std::string {
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

            static constexpr auto suitToString(Suit suit) -> std::string {
                switch (suit) {
                    case Suit::HEART: return "Heart";
                    case Suit::DIAMOND: return "Diamond";
                    case Suit::CLUB: return "Club";
                    case Suit::SPADE: return "Spade";
                    default: throw std::invalid_argument("The given suit is invalid");
                }
            }

        public:
            Card() : rank(Rank::UNDEFINED), suit(Suit::UNKNOWN){};
            Card(const Card& other) = default;
            Card(Card&& other) noexcept : rank(other.rank), suit(other.suit){};
            Card(Rank rank, Suit suit) : rank(rank), suit(suit){};

            virtual ~Card() = default;

            auto operator=(const Card& other) -> Card& = default;
            auto operator=(Card&& other) noexcept -> Card&;

            auto operator==(const Card& other) const -> bool { return std::tie(rank, suit) == std::tie(other.rank, other.suit); }
            auto operator!=(const Card& other) const -> bool { return !(other == *this); }

            [[nodiscard]] auto getRank() const -> Rank { return rank; }
            [[nodiscard]] auto getSuit() const -> Suit { return suit; }
            [[nodiscard]] auto getFullName() const -> std::string;
            [[nodiscard]] auto getShortName() const -> std::string;

            [[nodiscard]] auto toJson() const -> json;

        private:
            Rank rank;
            Suit suit;
    };
}  // namespace GameHandler