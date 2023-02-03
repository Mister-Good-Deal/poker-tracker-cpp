#pragma once

#include <map>

#include "Card.hpp"

namespace GameHandler::Factory {
    using Rank = Card::Rank;
    using Suit = Card::Suit;

    class invalid_card : public std::runtime_error {
        public:
            explicit invalid_card(const std::string& arg) : runtime_error(arg){};
    };

    static const std::map<std::string, Card> CARD_PROTOTYPES = {
        // Hearts
        {"2H", Card(Rank::TWO, Suit::HEART)},
        {"3H", Card(Rank::THREE, Suit::HEART)},
        {"4H", Card(Rank::FOUR, Suit::HEART)},
        {"5H", Card(Rank::FIVE, Suit::HEART)},
        {"6H", Card(Rank::SIX, Suit::HEART)},
        {"7H", Card(Rank::SEVEN, Suit::HEART)},
        {"8H", Card(Rank::EIGHT, Suit::HEART)},
        {"9H", Card(Rank::NINE, Suit::HEART)},
        {"TH", Card(Rank::TEN, Suit::HEART)},
        {"JH", Card(Rank::JACK, Suit::HEART)},
        {"QH", Card(Rank::QUEEN, Suit::HEART)},
        {"KH", Card(Rank::KING, Suit::HEART)},
        {"AH", Card(Rank::ACE, Suit::HEART)},
        // Diamonds
        {"2D", Card(Rank::TWO, Suit::DIAMOND)},
        {"3D", Card(Rank::THREE, Suit::DIAMOND)},
        {"4D", Card(Rank::FOUR, Suit::DIAMOND)},
        {"5D", Card(Rank::FIVE, Suit::DIAMOND)},
        {"6D", Card(Rank::SIX, Suit::DIAMOND)},
        {"7D", Card(Rank::SEVEN, Suit::DIAMOND)},
        {"8D", Card(Rank::EIGHT, Suit::DIAMOND)},
        {"9D", Card(Rank::NINE, Suit::DIAMOND)},
        {"TD", Card(Rank::TEN, Suit::DIAMOND)},
        {"JD", Card(Rank::JACK, Suit::DIAMOND)},
        {"QD", Card(Rank::QUEEN, Suit::DIAMOND)},
        {"KD", Card(Rank::KING, Suit::DIAMOND)},
        {"AD", Card(Rank::ACE, Suit::DIAMOND)},
        // Clubs
        {"2C", Card(Rank::TWO, Suit::CLUB)},
        {"3C", Card(Rank::THREE, Suit::CLUB)},
        {"4C", Card(Rank::FOUR, Suit::CLUB)},
        {"5C", Card(Rank::FIVE, Suit::CLUB)},
        {"6C", Card(Rank::SIX, Suit::CLUB)},
        {"7C", Card(Rank::SEVEN, Suit::CLUB)},
        {"8C", Card(Rank::EIGHT, Suit::CLUB)},
        {"9C", Card(Rank::NINE, Suit::CLUB)},
        {"TC", Card(Rank::TEN, Suit::CLUB)},
        {"JC", Card(Rank::JACK, Suit::CLUB)},
        {"QC", Card(Rank::QUEEN, Suit::CLUB)},
        {"KC", Card(Rank::KING, Suit::CLUB)},
        {"AC", Card(Rank::ACE, Suit::CLUB)},
        // Spade
        {"2S", Card(Rank::TWO, Suit::SPADE)},
        {"3S", Card(Rank::THREE, Suit::SPADE)},
        {"4S", Card(Rank::FOUR, Suit::SPADE)},
        {"5S", Card(Rank::FIVE, Suit::SPADE)},
        {"6S", Card(Rank::SIX, Suit::SPADE)},
        {"7S", Card(Rank::SEVEN, Suit::SPADE)},
        {"8S", Card(Rank::EIGHT, Suit::SPADE)},
        {"9S", Card(Rank::NINE, Suit::SPADE)},
        {"TS", Card(Rank::TEN, Suit::SPADE)},
        {"JS", Card(Rank::JACK, Suit::SPADE)},
        {"QS", Card(Rank::QUEEN, Suit::SPADE)},
        {"KS", Card(Rank::KING, Suit::SPADE)},
        {"AS", Card(Rank::ACE, Suit::SPADE)}};

    class CardFactory {
        public:
            CardFactory() = delete;

            static auto create(const std::string& cardName) -> Card;
    };
}  // namespace GameHandler::Factory
