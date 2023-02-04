#pragma once

#include <map>

#include "Card.hpp"

namespace GameHandler::Factory {
    using enum Card::Rank;
    using enum Card::Suit;

    class invalid_card : public std::runtime_error {
        public:
            explicit invalid_card(const std::string& arg) : runtime_error(arg){};
    };

    static const std::map<std::string, Card> CARD_PROTOTYPES = {
        // Hearts
        {"2H", Card(TWO, HEART)},
        {"3H", Card(THREE, HEART)},
        {"4H", Card(FOUR, HEART)},
        {"5H", Card(FIVE, HEART)},
        {"6H", Card(SIX, HEART)},
        {"7H", Card(SEVEN, HEART)},
        {"8H", Card(EIGHT, HEART)},
        {"9H", Card(NINE, HEART)},
        {"TH", Card(TEN, HEART)},
        {"JH", Card(JACK, HEART)},
        {"QH", Card(QUEEN, HEART)},
        {"KH", Card(KING, HEART)},
        {"AH", Card(ACE, HEART)},
        // Diamonds
        {"2D", Card(TWO, DIAMOND)},
        {"3D", Card(THREE, DIAMOND)},
        {"4D", Card(FOUR, DIAMOND)},
        {"5D", Card(FIVE, DIAMOND)},
        {"6D", Card(SIX, DIAMOND)},
        {"7D", Card(SEVEN, DIAMOND)},
        {"8D", Card(EIGHT, DIAMOND)},
        {"9D", Card(NINE, DIAMOND)},
        {"TD", Card(TEN, DIAMOND)},
        {"JD", Card(JACK, DIAMOND)},
        {"QD", Card(QUEEN, DIAMOND)},
        {"KD", Card(KING, DIAMOND)},
        {"AD", Card(ACE, DIAMOND)},
        // Clubs
        {"2C", Card(TWO, CLUB)},
        {"3C", Card(THREE, CLUB)},
        {"4C", Card(FOUR, CLUB)},
        {"5C", Card(FIVE, CLUB)},
        {"6C", Card(SIX, CLUB)},
        {"7C", Card(SEVEN, CLUB)},
        {"8C", Card(EIGHT, CLUB)},
        {"9C", Card(NINE, CLUB)},
        {"TC", Card(TEN, CLUB)},
        {"JC", Card(JACK, CLUB)},
        {"QC", Card(QUEEN, CLUB)},
        {"KC", Card(KING, CLUB)},
        {"AC", Card(ACE, CLUB)},
        // Spade
        {"2S", Card(TWO, SPADE)},
        {"3S", Card(THREE, SPADE)},
        {"4S", Card(FOUR, SPADE)},
        {"5S", Card(FIVE, SPADE)},
        {"6S", Card(SIX, SPADE)},
        {"7S", Card(SEVEN, SPADE)},
        {"8S", Card(EIGHT, SPADE)},
        {"9S", Card(NINE, SPADE)},
        {"TS", Card(TEN, SPADE)},
        {"JS", Card(JACK, SPADE)},
        {"QS", Card(QUEEN, SPADE)},
        {"KS", Card(KING, SPADE)},
        {"AS", Card(ACE, SPADE)}};

    class CardFactory {
        public:
            CardFactory() = delete;

            static auto create(const std::string& cardName) -> Card;
    };

    // Shortcut to call the factory
    const std::function<Card(const std::string&)> card = CardFactory::create;
}  // namespace GameHandler::Factory
