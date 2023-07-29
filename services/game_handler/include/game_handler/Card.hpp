#pragma once

#include <iostream>
#include <string>
#include <tuple>

#include <nlohmann/json.hpp>

#include <logger/Logger.hpp>

namespace GameHandler {
    using json   = nlohmann::json;
    using Logger = Logger::Quill;

    static const int32_t RANK_CARDS_NUMBER = 13;
    static const int32_t SUIT_CARDS_NUMBER = 4;
    static const int32_t BROADWAY_NUMBER   = 5;

    class Card {
        public:
            enum Rank : int32_t { TWO = 1, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE, UNDEFINED };

            enum Suit : int32_t { HEART = 0, DIAMOND, CLUB, SPADE, UNKNOWN };

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

            // @todo put constexpr when c++23
            static auto charToRank(char rank) -> Rank {
                switch (rank) {
                    case '2': return Rank::TWO;
                    case '3': return Rank::THREE;
                    case '4': return Rank::FOUR;
                    case '5': return Rank::FIVE;
                    case '6': return Rank::SIX;
                    case '7': return Rank::SEVEN;
                    case '8': return Rank::EIGHT;
                    case '9': return Rank::NINE;
                    case 'T': return Rank::TEN;
                    case 'J': return Rank::JACK;
                    case 'Q': return Rank::QUEEN;
                    case 'K': return Rank::KING;
                    case 'A': return Rank::ACE;
                    default:
                        LOG_ERROR(Logger::getLogger(), "The given rank char ASCII({}) is invalid", static_cast<int>(rank));

                        return Rank::UNDEFINED;
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
            Card() : _rank(Rank::UNDEFINED), _suit(Suit::UNKNOWN){};
            Card(const Card& other)     = default;
            Card(Card&& other) noexcept = default;
            Card(Rank rank, Suit suit) : _rank(rank), _suit(suit){};

            virtual ~Card() = default;

            auto operator=(const Card& other) -> Card& = default;
            auto operator=(Card&& other) noexcept -> Card&;

            auto operator==(const Card& other) const -> bool { return std::tie(_rank, _suit) == std::tie(other._rank, other._suit); }
            auto operator!=(const Card& other) const -> bool { return !(other == *this); }

            [[nodiscard]] auto getRank() const -> Rank { return _rank; }
            [[nodiscard]] auto getSuit() const -> Suit { return _suit; }
            [[nodiscard]] auto getFullName() const -> std::string;
            [[nodiscard]] auto getShortName() const -> std::string;
            [[nodiscard]] auto isBroadway() const -> bool;
            [[nodiscard]] auto isUnknown() const -> bool { return _suit == Suit::UNKNOWN || _rank == Rank::UNDEFINED; };

            [[nodiscard]] auto toJson() const -> json;

        private:
            Rank _rank;
            Suit _suit;
    };

    using enum Card::Rank;

    static constexpr std::array<Card::Rank, BROADWAY_NUMBER> BROADWAY = {TEN, JACK, QUEEN, KING, ACE};
}  // namespace GameHandler