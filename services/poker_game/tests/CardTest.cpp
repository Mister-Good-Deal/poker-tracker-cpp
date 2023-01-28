#include "Card.hpp"
#include "gtest/gtest.h"

#define EXPECT_STD_STREQ(str1, str2) EXPECT_STREQ(str1.c_str(), str2.c_str())

using Rank = Card::Rank;
using Suit = Card::Suit;

class CardTest : public ::testing::Test {};

TEST(CardTest, Constructor) {
    Card card(Card::Rank::ACE, Card::Suit::SPADE);

    EXPECT_EQ(Card::Suit::SPADE, card.getSuit());
    EXPECT_EQ(Card::Rank::ACE, card.getRank());
}

TEST(CardTest, ExpectCorrectFullName) {
    EXPECT_STREQ(Card(Rank::TWO, Suit::HEART).getFullName().c_str(), "Two of Heart");
    EXPECT_STREQ(Card(Rank::THREE, Suit::HEART).getFullName().c_str(), "Three of Heart");
    EXPECT_STREQ(Card(Rank::FOUR, Suit::HEART).getFullName().c_str(), "Four of Heart");
    EXPECT_STREQ(Card(Rank::FIVE, Suit::HEART).getFullName().c_str(), "Five of Heart");
    EXPECT_STREQ(Card(Rank::SIX, Suit::HEART).getFullName().c_str(), "Six of Heart");
    EXPECT_STREQ(Card(Rank::SEVEN, Suit::HEART).getFullName().c_str(), "Seven of Heart");
    EXPECT_STREQ(Card(Rank::EIGHT, Suit::HEART).getFullName().c_str(), "Eight of Heart");
    EXPECT_STREQ(Card(Rank::NINE, Suit::HEART).getFullName().c_str(), "Nine of Heart");
    EXPECT_STREQ(Card(Rank::TEN, Suit::HEART).getFullName().c_str(), "Ten of Heart");
    EXPECT_STREQ(Card(Rank::JACK, Suit::HEART).getFullName().c_str(), "Jack of Heart");
    EXPECT_STREQ(Card(Rank::QUEEN, Suit::HEART).getFullName().c_str(), "Queen of Heart");
    EXPECT_STREQ(Card(Rank::KING, Suit::HEART).getFullName().c_str(), "King of Heart");
    EXPECT_STREQ(Card(Rank::ACE, Suit::HEART).getFullName().c_str(), "Ace of Heart");

    EXPECT_STREQ(Card(Rank::TWO, Suit::DIAMOND).getFullName().c_str(), "Two of Diamond");
    EXPECT_STREQ(Card(Rank::THREE, Suit::DIAMOND).getFullName().c_str(), "Three of Diamond");
    EXPECT_STREQ(Card(Rank::FOUR, Suit::DIAMOND).getFullName().c_str(), "Four of Diamond");
    EXPECT_STREQ(Card(Rank::FIVE, Suit::DIAMOND).getFullName().c_str(), "Five of Diamond");
    EXPECT_STREQ(Card(Rank::SIX, Suit::DIAMOND).getFullName().c_str(), "Six of Diamond");
    EXPECT_STREQ(Card(Rank::SEVEN, Suit::DIAMOND).getFullName().c_str(), "Seven of Diamond");
    EXPECT_STREQ(Card(Rank::EIGHT, Suit::DIAMOND).getFullName().c_str(), "Eight of Diamond");
    EXPECT_STREQ(Card(Rank::NINE, Suit::DIAMOND).getFullName().c_str(), "Nine of Diamond");
    EXPECT_STREQ(Card(Rank::TEN, Suit::DIAMOND).getFullName().c_str(), "Ten of Diamond");
    EXPECT_STREQ(Card(Rank::JACK, Suit::DIAMOND).getFullName().c_str(), "Jack of Diamond");
    EXPECT_STREQ(Card(Rank::QUEEN, Suit::DIAMOND).getFullName().c_str(), "Queen of Diamond");
    EXPECT_STREQ(Card(Rank::KING, Suit::DIAMOND).getFullName().c_str(), "King of Diamond");
    EXPECT_STREQ(Card(Rank::ACE, Suit::DIAMOND).getFullName().c_str(), "Ace of Diamond");

    EXPECT_STREQ(Card(Rank::TWO, Suit::CLUB).getFullName().c_str(), "Two of Club");
    EXPECT_STREQ(Card(Rank::THREE, Suit::CLUB).getFullName().c_str(), "Three of Club");
    EXPECT_STREQ(Card(Rank::FOUR, Suit::CLUB).getFullName().c_str(), "Four of Club");
    EXPECT_STREQ(Card(Rank::FIVE, Suit::CLUB).getFullName().c_str(), "Five of Club");
    EXPECT_STREQ(Card(Rank::SIX, Suit::CLUB).getFullName().c_str(), "Six of Club");
    EXPECT_STREQ(Card(Rank::SEVEN, Suit::CLUB).getFullName().c_str(), "Seven of Club");
    EXPECT_STREQ(Card(Rank::EIGHT, Suit::CLUB).getFullName().c_str(), "Eight of Club");
    EXPECT_STREQ(Card(Rank::NINE, Suit::CLUB).getFullName().c_str(), "Nine of Club");
    EXPECT_STREQ(Card(Rank::TEN, Suit::CLUB).getFullName().c_str(), "Ten of Club");
    EXPECT_STREQ(Card(Rank::JACK, Suit::CLUB).getFullName().c_str(), "Jack of Club");
    EXPECT_STREQ(Card(Rank::QUEEN, Suit::CLUB).getFullName().c_str(), "Queen of Club");
    EXPECT_STREQ(Card(Rank::KING, Suit::CLUB).getFullName().c_str(), "King of Club");
    EXPECT_STREQ(Card(Rank::ACE, Suit::CLUB).getFullName().c_str(), "Ace of Club");

    EXPECT_STREQ(Card(Rank::TWO, Suit::SPADE).getFullName().c_str(), "Two of Spade");
    EXPECT_STREQ(Card(Rank::THREE, Suit::SPADE).getFullName().c_str(), "Three of Spade");
    EXPECT_STREQ(Card(Rank::FOUR, Suit::SPADE).getFullName().c_str(), "Four of Spade");
    EXPECT_STREQ(Card(Rank::FIVE, Suit::SPADE).getFullName().c_str(), "Five of Spade");
    EXPECT_STREQ(Card(Rank::SIX, Suit::SPADE).getFullName().c_str(), "Six of Spade");
    EXPECT_STREQ(Card(Rank::SEVEN, Suit::SPADE).getFullName().c_str(), "Seven of Spade");
    EXPECT_STREQ(Card(Rank::EIGHT, Suit::SPADE).getFullName().c_str(), "Eight of Spade");
    EXPECT_STREQ(Card(Rank::NINE, Suit::SPADE).getFullName().c_str(), "Nine of Spade");
    EXPECT_STREQ(Card(Rank::TEN, Suit::SPADE).getFullName().c_str(), "Ten of Spade");
    EXPECT_STREQ(Card(Rank::JACK, Suit::SPADE).getFullName().c_str(), "Jack of Spade");
    EXPECT_STREQ(Card(Rank::QUEEN, Suit::SPADE).getFullName().c_str(), "Queen of Spade");
    EXPECT_STREQ(Card(Rank::KING, Suit::SPADE).getFullName().c_str(), "King of Spade");
    EXPECT_STREQ(Card(Rank::ACE, Suit::SPADE).getFullName().c_str(), "Ace of Spade");
}

TEST(CardTest, ExpectCorrectShortName) {
    EXPECT_STREQ(Card(Rank::TWO, Suit::HEART).getShortName().c_str(), "2H");
    EXPECT_STREQ(Card(Rank::THREE, Suit::HEART).getShortName().c_str(), "3H");
    EXPECT_STREQ(Card(Rank::FOUR, Suit::HEART).getShortName().c_str(), "4H");
    EXPECT_STREQ(Card(Rank::FIVE, Suit::HEART).getShortName().c_str(), "5H");
    EXPECT_STREQ(Card(Rank::SIX, Suit::HEART).getShortName().c_str(), "6H");
    EXPECT_STREQ(Card(Rank::SEVEN, Suit::HEART).getShortName().c_str(), "7H");
    EXPECT_STREQ(Card(Rank::EIGHT, Suit::HEART).getShortName().c_str(), "8H");
    EXPECT_STREQ(Card(Rank::NINE, Suit::HEART).getShortName().c_str(), "9H");
    EXPECT_STREQ(Card(Rank::TEN, Suit::HEART).getShortName().c_str(), "TH");
    EXPECT_STREQ(Card(Rank::JACK, Suit::HEART).getShortName().c_str(), "JH");
    EXPECT_STREQ(Card(Rank::QUEEN, Suit::HEART).getShortName().c_str(), "QH");
    EXPECT_STREQ(Card(Rank::KING, Suit::HEART).getShortName().c_str(), "KH");
    EXPECT_STREQ(Card(Rank::ACE, Suit::HEART).getShortName().c_str(), "AH");

    EXPECT_STREQ(Card(Rank::TWO, Suit::DIAMOND).getShortName().c_str(), "2D");
    EXPECT_STREQ(Card(Rank::THREE, Suit::DIAMOND).getShortName().c_str(), "3D");
    EXPECT_STREQ(Card(Rank::FOUR, Suit::DIAMOND).getShortName().c_str(), "4D");
    EXPECT_STREQ(Card(Rank::FIVE, Suit::DIAMOND).getShortName().c_str(), "5D");
    EXPECT_STREQ(Card(Rank::SIX, Suit::DIAMOND).getShortName().c_str(), "6D");
    EXPECT_STREQ(Card(Rank::SEVEN, Suit::DIAMOND).getShortName().c_str(), "7D");
    EXPECT_STREQ(Card(Rank::EIGHT, Suit::DIAMOND).getShortName().c_str(), "8D");
    EXPECT_STREQ(Card(Rank::NINE, Suit::DIAMOND).getShortName().c_str(), "9D");
    EXPECT_STREQ(Card(Rank::TEN, Suit::DIAMOND).getShortName().c_str(), "TD");
    EXPECT_STREQ(Card(Rank::JACK, Suit::DIAMOND).getShortName().c_str(), "JD");
    EXPECT_STREQ(Card(Rank::QUEEN, Suit::DIAMOND).getShortName().c_str(), "QD");
    EXPECT_STREQ(Card(Rank::KING, Suit::DIAMOND).getShortName().c_str(), "KD");
    EXPECT_STREQ(Card(Rank::ACE, Suit::DIAMOND).getShortName().c_str(), "AD");

    EXPECT_STREQ(Card(Rank::TWO, Suit::CLUB).getShortName().c_str(), "2C");
    EXPECT_STREQ(Card(Rank::THREE, Suit::CLUB).getShortName().c_str(), "3C");
    EXPECT_STREQ(Card(Rank::FOUR, Suit::CLUB).getShortName().c_str(), "4C");
    EXPECT_STREQ(Card(Rank::FIVE, Suit::CLUB).getShortName().c_str(), "5C");
    EXPECT_STREQ(Card(Rank::SIX, Suit::CLUB).getShortName().c_str(), "6C");
    EXPECT_STREQ(Card(Rank::SEVEN, Suit::CLUB).getShortName().c_str(), "7C");
    EXPECT_STREQ(Card(Rank::EIGHT, Suit::CLUB).getShortName().c_str(), "8C");
    EXPECT_STREQ(Card(Rank::NINE, Suit::CLUB).getShortName().c_str(), "9C");
    EXPECT_STREQ(Card(Rank::TEN, Suit::CLUB).getShortName().c_str(), "TC");
    EXPECT_STREQ(Card(Rank::JACK, Suit::CLUB).getShortName().c_str(), "JC");
    EXPECT_STREQ(Card(Rank::QUEEN, Suit::CLUB).getShortName().c_str(), "QC");
    EXPECT_STREQ(Card(Rank::KING, Suit::CLUB).getShortName().c_str(), "KC");
    EXPECT_STREQ(Card(Rank::ACE, Suit::CLUB).getShortName().c_str(), "AC");

    EXPECT_STREQ(Card(Rank::TWO, Suit::SPADE).getShortName().c_str(), "2S");
    EXPECT_STREQ(Card(Rank::THREE, Suit::SPADE).getShortName().c_str(), "3S");
    EXPECT_STREQ(Card(Rank::FOUR, Suit::SPADE).getShortName().c_str(), "4S");
    EXPECT_STREQ(Card(Rank::FIVE, Suit::SPADE).getShortName().c_str(), "5S");
    EXPECT_STREQ(Card(Rank::SIX, Suit::SPADE).getShortName().c_str(), "6S");
    EXPECT_STREQ(Card(Rank::SEVEN, Suit::SPADE).getShortName().c_str(), "7S");
    EXPECT_STREQ(Card(Rank::EIGHT, Suit::SPADE).getShortName().c_str(), "8S");
    EXPECT_STREQ(Card(Rank::NINE, Suit::SPADE).getShortName().c_str(), "9S");
    EXPECT_STREQ(Card(Rank::TEN, Suit::SPADE).getShortName().c_str(), "TS");
    EXPECT_STREQ(Card(Rank::JACK, Suit::SPADE).getShortName().c_str(), "JS");
    EXPECT_STREQ(Card(Rank::QUEEN, Suit::SPADE).getShortName().c_str(), "QS");
    EXPECT_STREQ(Card(Rank::KING, Suit::SPADE).getShortName().c_str(), "KS");
    EXPECT_STREQ(Card(Rank::ACE, Suit::SPADE).getShortName().c_str(), "AS");
}
