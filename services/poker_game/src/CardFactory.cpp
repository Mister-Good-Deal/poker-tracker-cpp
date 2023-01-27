#include "CardFactory.hpp"

#include <stdexcept>

#include "Card.hpp"

using Rank = Card::Rank;
using Suit = Card::Suit;

CardFactory::CardFactory() {
    cardPrototypes["2H"] = Card(Rank::TWO, Suit::HEART);
    cardPrototypes["3H"] = Card(Rank::THREE, Suit::HEART);
    cardPrototypes["4H"] = Card(Rank::FOUR, Suit::HEART);
    cardPrototypes["5H"] = Card(Rank::FIVE, Suit::HEART);
    cardPrototypes["6H"] = Card(Rank::SIX, Suit::HEART);
    cardPrototypes["7H"] = Card(Rank::SEVEN, Suit::HEART);
    cardPrototypes["8H"] = Card(Rank::EIGHT, Suit::HEART);
    cardPrototypes["9H"] = Card(Rank::NINE, Suit::HEART);
    cardPrototypes["TH"] = Card(Rank::TEN, Suit::HEART);
    cardPrototypes["JH"] = Card(Rank::JACK, Suit::HEART);
    cardPrototypes["QH"] = Card(Rank::QUEEN, Suit::HEART);
    cardPrototypes["KH"] = Card(Rank::KING, Suit::HEART);
    cardPrototypes["AH"] = Card(Rank::ACE, Suit::HEART);

    cardPrototypes["2D"] = Card(Rank::TWO, Suit::DIAMOND);
    cardPrototypes["3D"] = Card(Rank::THREE, Suit::DIAMOND);
    cardPrototypes["4D"] = Card(Rank::FOUR, Suit::DIAMOND);
    cardPrototypes["5D"] = Card(Rank::FIVE, Suit::DIAMOND);
    cardPrototypes["6D"] = Card(Rank::SIX, Suit::DIAMOND);
    cardPrototypes["7D"] = Card(Rank::SEVEN, Suit::DIAMOND);
    cardPrototypes["8D"] = Card(Rank::EIGHT, Suit::DIAMOND);
    cardPrototypes["9D"] = Card(Rank::NINE, Suit::DIAMOND);
    cardPrototypes["TD"] = Card(Rank::TEN, Suit::DIAMOND);
    cardPrototypes["JD"] = Card(Rank::JACK, Suit::DIAMOND);
    cardPrototypes["QD"] = Card(Rank::QUEEN, Suit::DIAMOND);
    cardPrototypes["KD"] = Card(Rank::KING, Suit::DIAMOND);
    cardPrototypes["AD"] = Card(Rank::ACE, Suit::DIAMOND);

    cardPrototypes["2S"] = Card(Rank::TWO, Suit::SPADE);
    cardPrototypes["3S"] = Card(Rank::THREE, Suit::SPADE);
    cardPrototypes["4S"] = Card(Rank::FOUR, Suit::SPADE);
    cardPrototypes["5S"] = Card(Rank::FIVE, Suit::SPADE);
    cardPrototypes["6S"] = Card(Rank::SIX, Suit::SPADE);
    cardPrototypes["7S"] = Card(Rank::SEVEN, Suit::SPADE);
    cardPrototypes["8S"] = Card(Rank::EIGHT, Suit::SPADE);
    cardPrototypes["9S"] = Card(Rank::NINE, Suit::SPADE);
    cardPrototypes["TS"] = Card(Rank::TEN, Suit::SPADE);
    cardPrototypes["JS"] = Card(Rank::JACK, Suit::SPADE);
    cardPrototypes["QS"] = Card(Rank::QUEEN, Suit::SPADE);
    cardPrototypes["KS"] = Card(Rank::KING, Suit::SPADE);
    cardPrototypes["AS"] = Card(Rank::ACE, Suit::SPADE);

    cardPrototypes["2C"] = Card(Rank::TWO, Suit::CLUB);
    cardPrototypes["3C"] = Card(Rank::THREE, Suit::CLUB);
    cardPrototypes["4C"] = Card(Rank::FOUR, Suit::CLUB);
    cardPrototypes["5C"] = Card(Rank::FIVE, Suit::CLUB);
    cardPrototypes["6C"] = Card(Rank::SIX, Suit::CLUB);
    cardPrototypes["7C"] = Card(Rank::SEVEN, Suit::CLUB);
    cardPrototypes["8C"] = Card(Rank::EIGHT, Suit::CLUB);
    cardPrototypes["9C"] = Card(Rank::NINE, Suit::CLUB);
    cardPrototypes["TC"] = Card(Rank::TEN, Suit::CLUB);
    cardPrototypes["JC"] = Card(Rank::JACK, Suit::CLUB);
    cardPrototypes["QC"] = Card(Rank::QUEEN, Suit::CLUB);
    cardPrototypes["KC"] = Card(Rank::KING, Suit::CLUB);
    cardPrototypes["AC"] = Card(Rank::ACE, Suit::CLUB);
}

Card CardFactory::create(const std::string& shortCardName) {
    if (cardPrototypes.count(shortCardName) == 0) {
        throw std::invalid_argument("Invalid short card name: " + shortCardName);
    }

    return cardPrototypes[shortCardName];
}
