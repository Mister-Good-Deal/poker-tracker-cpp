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

    class UnknownCardRankException : public std::exception {
        public:
            explicit UnknownCardRankException(char rank)
              : _rank(rank)
              , _message(fmt::format("Unknown card rank `{}`", _rank)) {}

            [[nodiscard]] auto what() const noexcept -> const char* override { return _message.c_str(); }

        private:
            char        _rank;
            std::string _message;
    };

    class Card {
        public:
            enum Rank : int32_t { TWO = 1, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE, UNDEFINED };

            enum Suit : int32_t { HEART = 0, DIAMOND, CLUB, SPADE, UNKNOWN };

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
                    case '1':  // Can be 10 or T
                    case 'T': return Rank::TEN;
                    case 'J': return Rank::JACK;
                    case 'Q': return Rank::QUEEN;
                    case 'K': return Rank::KING;
                    case 'A': return Rank::ACE;
                    default: throw UnknownCardRankException(rank);
                }
            }

        public:
            Card()
              : _rank(Rank::UNDEFINED)
              , _suit(Suit::UNKNOWN) {};
            Card(const Card& other)     = default;
            Card(Card&& other) noexcept = default;
            Card(Rank rank, Suit suit)
              : _rank(rank)
              , _suit(suit) {};

            virtual ~Card() = default;

            auto operator=(const Card& other) -> Card& = default;
            auto operator=(Card&& other) noexcept -> Card&;

            auto operator==(const Card& other) const -> bool { return std::tie(_rank, _suit) == std::tie(other._rank, other._suit); }
            auto operator!=(const Card& other) const -> bool { return !(other == *this); }

            [[nodiscard]] auto getRank() const -> Rank { return _rank; }
            [[nodiscard]] auto getSuit() const -> Suit { return _suit; }
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

// Custom formatter for Rank, Suit and Card
namespace fmt {
    using GameHandler::Card;

    template<> struct formatter<Card::Rank> : formatter<string_view> {
            char presentation = 's';  // Default presentation type: short

            // Parsing function to support different presentations short and long
            constexpr auto parse(auto& ctx) {
                auto it = ctx.begin(), end = ctx.end();

                if (it != end && (*it == 's' || *it == 'l')) { presentation = *it++; }
                if (it != end && *it != '}') { throw format_error("invalid format"); }

                return it;
            }

            template<typename FormatContext> auto format(Card::Rank rank, FormatContext& ctx) const {
                string_view name = "";

                switch (rank) {
                    case Card::Rank::TWO: name = (presentation == 's') ? "2" : "Two"; break;
                    case Card::Rank::THREE: name = (presentation == 's') ? "3" : "Three"; break;
                    case Card::Rank::FOUR: name = (presentation == 's') ? "4" : "Four"; break;
                    case Card::Rank::FIVE: name = (presentation == 's') ? "5" : "Five"; break;
                    case Card::Rank::SIX: name = (presentation == 's') ? "6" : "Six"; break;
                    case Card::Rank::SEVEN: name = (presentation == 's') ? "7" : "Seven"; break;
                    case Card::Rank::EIGHT: name = (presentation == 's') ? "8" : "Eight"; break;
                    case Card::Rank::NINE: name = (presentation == 's') ? "9" : "Nine"; break;
                    case Card::Rank::TEN: name = (presentation == 's') ? "T" : "Ten"; break;
                    case Card::Rank::JACK: name = (presentation == 's') ? "J" : "Jack"; break;
                    case Card::Rank::QUEEN: name = (presentation == 's') ? "Q" : "Queen"; break;
                    case Card::Rank::KING: name = (presentation == 's') ? "K" : "King"; break;
                    case Card::Rank::ACE: name = (presentation == 's') ? "A" : "Ace"; break;
                    case Card::Rank::UNDEFINED: break;
                }

                return formatter<string_view>::format(name, ctx);
            }
    };

    template<> struct formatter<Card::Suit> : formatter<string_view> {
            char presentation = 's';  // Default presentation type: short

            // Parsing function to support different presentations short and long
            constexpr auto parse(auto& ctx) {
                auto it = ctx.begin(), end = ctx.end();

                if (it != end && (*it == 's' || *it == 'l')) { presentation = *it++; }
                if (it != end && *it != '}') { throw format_error("invalid format"); }

                return it;
            }

            template<typename FormatContext> auto format(Card::Suit suit, FormatContext& ctx) const {
                string_view name = "";

                switch (suit) {
                    case Card::Suit::HEART: name = (presentation == 's') ? "H" : "Heart"; break;
                    case Card::Suit::DIAMOND: name = (presentation == 's') ? "D" : "Diamond"; break;
                    case Card::Suit::CLUB: name = (presentation == 's') ? "C" : "Club"; break;
                    case Card::Suit::SPADE: name = (presentation == 's') ? "S" : "Spade"; break;
                    case Card::Suit::UNKNOWN: break;
                }

                return formatter<string_view>::format(name, ctx);
            }
    };

    template<> struct formatter<Card> : formatter<string_view> {
            char presentation = 's';  // Default presentation type: short

            // Parsing function to support different presentations short and long
            constexpr auto parse(auto& ctx) {
                auto it = ctx.begin(), end = ctx.end();

                if (it != end && (*it == 's' || *it == 'l')) { presentation = *it++; }
                if (it != end && *it != '}') { throw format_error("invalid format"); }

                return it;
            }

            template<typename FormatContext> auto format(const Card& card, FormatContext& ctx) const {
                return presentation == 's' ? fmt::format_to(ctx.out(), "{:s}{:s}", card.getRank(), card.getSuit())
                                           : fmt::format_to(ctx.out(), "{:l} of {:l}", card.getRank(), card.getSuit());
            }
    };
}  // namespace fmt

// Registered as safe to copy for Quill logger
namespace quill {
    template<> struct copy_loggable<GameHandler::Card> : std::true_type {};
}  // namespace quill