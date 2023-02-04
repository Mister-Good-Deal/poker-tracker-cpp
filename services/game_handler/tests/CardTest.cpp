#include "Card.hpp"
#include "gtest/gtest.h"

using GameHandler::Card;

using enum Card::Rank;
using enum Card::Suit;

class CardTest : public ::testing::Test {};

TEST(CardTest, ConstructorShouldSetRankAndSuitCorrectly) {
    Card card(ACE, SPADE);

    EXPECT_EQ(SPADE, card.getSuit());
    EXPECT_EQ(ACE, card.getRank());
}

TEST(CardTest, FullNamesShouldBeCorrect) {
    // Hearts
    EXPECT_STREQ(Card(TWO, HEART).getFullName().c_str(), "Two of Heart");
    EXPECT_STREQ(Card(THREE, HEART).getFullName().c_str(), "Three of Heart");
    EXPECT_STREQ(Card(FOUR, HEART).getFullName().c_str(), "Four of Heart");
    EXPECT_STREQ(Card(FIVE, HEART).getFullName().c_str(), "Five of Heart");
    EXPECT_STREQ(Card(SIX, HEART).getFullName().c_str(), "Six of Heart");
    EXPECT_STREQ(Card(SEVEN, HEART).getFullName().c_str(), "Seven of Heart");
    EXPECT_STREQ(Card(EIGHT, HEART).getFullName().c_str(), "Eight of Heart");
    EXPECT_STREQ(Card(NINE, HEART).getFullName().c_str(), "Nine of Heart");
    EXPECT_STREQ(Card(TEN, HEART).getFullName().c_str(), "Ten of Heart");
    EXPECT_STREQ(Card(JACK, HEART).getFullName().c_str(), "Jack of Heart");
    EXPECT_STREQ(Card(QUEEN, HEART).getFullName().c_str(), "Queen of Heart");
    EXPECT_STREQ(Card(KING, HEART).getFullName().c_str(), "King of Heart");
    EXPECT_STREQ(Card(ACE, HEART).getFullName().c_str(), "Ace of Heart");
    // Diamonds
    EXPECT_STREQ(Card(TWO, DIAMOND).getFullName().c_str(), "Two of Diamond");
    EXPECT_STREQ(Card(THREE, DIAMOND).getFullName().c_str(), "Three of Diamond");
    EXPECT_STREQ(Card(FOUR, DIAMOND).getFullName().c_str(), "Four of Diamond");
    EXPECT_STREQ(Card(FIVE, DIAMOND).getFullName().c_str(), "Five of Diamond");
    EXPECT_STREQ(Card(SIX, DIAMOND).getFullName().c_str(), "Six of Diamond");
    EXPECT_STREQ(Card(SEVEN, DIAMOND).getFullName().c_str(), "Seven of Diamond");
    EXPECT_STREQ(Card(EIGHT, DIAMOND).getFullName().c_str(), "Eight of Diamond");
    EXPECT_STREQ(Card(NINE, DIAMOND).getFullName().c_str(), "Nine of Diamond");
    EXPECT_STREQ(Card(TEN, DIAMOND).getFullName().c_str(), "Ten of Diamond");
    EXPECT_STREQ(Card(JACK, DIAMOND).getFullName().c_str(), "Jack of Diamond");
    EXPECT_STREQ(Card(QUEEN, DIAMOND).getFullName().c_str(), "Queen of Diamond");
    EXPECT_STREQ(Card(KING, DIAMOND).getFullName().c_str(), "King of Diamond");
    EXPECT_STREQ(Card(ACE, DIAMOND).getFullName().c_str(), "Ace of Diamond");
    // Club
    EXPECT_STREQ(Card(TWO, CLUB).getFullName().c_str(), "Two of Club");
    EXPECT_STREQ(Card(THREE, CLUB).getFullName().c_str(), "Three of Club");
    EXPECT_STREQ(Card(FOUR, CLUB).getFullName().c_str(), "Four of Club");
    EXPECT_STREQ(Card(FIVE, CLUB).getFullName().c_str(), "Five of Club");
    EXPECT_STREQ(Card(SIX, CLUB).getFullName().c_str(), "Six of Club");
    EXPECT_STREQ(Card(SEVEN, CLUB).getFullName().c_str(), "Seven of Club");
    EXPECT_STREQ(Card(EIGHT, CLUB).getFullName().c_str(), "Eight of Club");
    EXPECT_STREQ(Card(NINE, CLUB).getFullName().c_str(), "Nine of Club");
    EXPECT_STREQ(Card(TEN, CLUB).getFullName().c_str(), "Ten of Club");
    EXPECT_STREQ(Card(JACK, CLUB).getFullName().c_str(), "Jack of Club");
    EXPECT_STREQ(Card(QUEEN, CLUB).getFullName().c_str(), "Queen of Club");
    EXPECT_STREQ(Card(KING, CLUB).getFullName().c_str(), "King of Club");
    EXPECT_STREQ(Card(ACE, CLUB).getFullName().c_str(), "Ace of Club");
    // Spade
    EXPECT_STREQ(Card(TWO, SPADE).getFullName().c_str(), "Two of Spade");
    EXPECT_STREQ(Card(THREE, SPADE).getFullName().c_str(), "Three of Spade");
    EXPECT_STREQ(Card(FOUR, SPADE).getFullName().c_str(), "Four of Spade");
    EXPECT_STREQ(Card(FIVE, SPADE).getFullName().c_str(), "Five of Spade");
    EXPECT_STREQ(Card(SIX, SPADE).getFullName().c_str(), "Six of Spade");
    EXPECT_STREQ(Card(SEVEN, SPADE).getFullName().c_str(), "Seven of Spade");
    EXPECT_STREQ(Card(EIGHT, SPADE).getFullName().c_str(), "Eight of Spade");
    EXPECT_STREQ(Card(NINE, SPADE).getFullName().c_str(), "Nine of Spade");
    EXPECT_STREQ(Card(TEN, SPADE).getFullName().c_str(), "Ten of Spade");
    EXPECT_STREQ(Card(JACK, SPADE).getFullName().c_str(), "Jack of Spade");
    EXPECT_STREQ(Card(QUEEN, SPADE).getFullName().c_str(), "Queen of Spade");
    EXPECT_STREQ(Card(KING, SPADE).getFullName().c_str(), "King of Spade");
    EXPECT_STREQ(Card(ACE, SPADE).getFullName().c_str(), "Ace of Spade");
}

TEST(CardTest, ShortNamesShouldBeCorrect) {
    // Hearts
    EXPECT_STREQ(Card(TWO, HEART).getShortName().c_str(), "2H");
    EXPECT_STREQ(Card(THREE, HEART).getShortName().c_str(), "3H");
    EXPECT_STREQ(Card(FOUR, HEART).getShortName().c_str(), "4H");
    EXPECT_STREQ(Card(FIVE, HEART).getShortName().c_str(), "5H");
    EXPECT_STREQ(Card(SIX, HEART).getShortName().c_str(), "6H");
    EXPECT_STREQ(Card(SEVEN, HEART).getShortName().c_str(), "7H");
    EXPECT_STREQ(Card(EIGHT, HEART).getShortName().c_str(), "8H");
    EXPECT_STREQ(Card(NINE, HEART).getShortName().c_str(), "9H");
    EXPECT_STREQ(Card(TEN, HEART).getShortName().c_str(), "TH");
    EXPECT_STREQ(Card(JACK, HEART).getShortName().c_str(), "JH");
    EXPECT_STREQ(Card(QUEEN, HEART).getShortName().c_str(), "QH");
    EXPECT_STREQ(Card(KING, HEART).getShortName().c_str(), "KH");
    EXPECT_STREQ(Card(ACE, HEART).getShortName().c_str(), "AH");
    // Diamonds
    EXPECT_STREQ(Card(TWO, DIAMOND).getShortName().c_str(), "2D");
    EXPECT_STREQ(Card(THREE, DIAMOND).getShortName().c_str(), "3D");
    EXPECT_STREQ(Card(FOUR, DIAMOND).getShortName().c_str(), "4D");
    EXPECT_STREQ(Card(FIVE, DIAMOND).getShortName().c_str(), "5D");
    EXPECT_STREQ(Card(SIX, DIAMOND).getShortName().c_str(), "6D");
    EXPECT_STREQ(Card(SEVEN, DIAMOND).getShortName().c_str(), "7D");
    EXPECT_STREQ(Card(EIGHT, DIAMOND).getShortName().c_str(), "8D");
    EXPECT_STREQ(Card(NINE, DIAMOND).getShortName().c_str(), "9D");
    EXPECT_STREQ(Card(TEN, DIAMOND).getShortName().c_str(), "TD");
    EXPECT_STREQ(Card(JACK, DIAMOND).getShortName().c_str(), "JD");
    EXPECT_STREQ(Card(QUEEN, DIAMOND).getShortName().c_str(), "QD");
    EXPECT_STREQ(Card(KING, DIAMOND).getShortName().c_str(), "KD");
    EXPECT_STREQ(Card(ACE, DIAMOND).getShortName().c_str(), "AD");
    // Club
    EXPECT_STREQ(Card(TWO, CLUB).getShortName().c_str(), "2C");
    EXPECT_STREQ(Card(THREE, CLUB).getShortName().c_str(), "3C");
    EXPECT_STREQ(Card(FOUR, CLUB).getShortName().c_str(), "4C");
    EXPECT_STREQ(Card(FIVE, CLUB).getShortName().c_str(), "5C");
    EXPECT_STREQ(Card(SIX, CLUB).getShortName().c_str(), "6C");
    EXPECT_STREQ(Card(SEVEN, CLUB).getShortName().c_str(), "7C");
    EXPECT_STREQ(Card(EIGHT, CLUB).getShortName().c_str(), "8C");
    EXPECT_STREQ(Card(NINE, CLUB).getShortName().c_str(), "9C");
    EXPECT_STREQ(Card(TEN, CLUB).getShortName().c_str(), "TC");
    EXPECT_STREQ(Card(JACK, CLUB).getShortName().c_str(), "JC");
    EXPECT_STREQ(Card(QUEEN, CLUB).getShortName().c_str(), "QC");
    EXPECT_STREQ(Card(KING, CLUB).getShortName().c_str(), "KC");
    EXPECT_STREQ(Card(ACE, CLUB).getShortName().c_str(), "AC");
    // Spade
    EXPECT_STREQ(Card(TWO, SPADE).getShortName().c_str(), "2S");
    EXPECT_STREQ(Card(THREE, SPADE).getShortName().c_str(), "3S");
    EXPECT_STREQ(Card(FOUR, SPADE).getShortName().c_str(), "4S");
    EXPECT_STREQ(Card(FIVE, SPADE).getShortName().c_str(), "5S");
    EXPECT_STREQ(Card(SIX, SPADE).getShortName().c_str(), "6S");
    EXPECT_STREQ(Card(SEVEN, SPADE).getShortName().c_str(), "7S");
    EXPECT_STREQ(Card(EIGHT, SPADE).getShortName().c_str(), "8S");
    EXPECT_STREQ(Card(NINE, SPADE).getShortName().c_str(), "9S");
    EXPECT_STREQ(Card(TEN, SPADE).getShortName().c_str(), "TS");
    EXPECT_STREQ(Card(JACK, SPADE).getShortName().c_str(), "JS");
    EXPECT_STREQ(Card(QUEEN, SPADE).getShortName().c_str(), "QS");
    EXPECT_STREQ(Card(KING, SPADE).getShortName().c_str(), "KS");
    EXPECT_STREQ(Card(ACE, SPADE).getShortName().c_str(), "AS");
}

TEST(CardTest, JsonRepresentationShouldBeCorrent) {
    // language=json
    auto expectedJson = R"(
        {
            "shortName": "TS",
            "fullName": "Ten of Spade",
            "rank": "Ten",
            "suit": "Spade"
        }
    )"_json;

    EXPECT_EQ(Card(TEN, SPADE).toJson(), expectedJson);
}
