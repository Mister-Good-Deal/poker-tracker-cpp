#include "CardFactory.hpp"
#include "gtest/gtest.h"

using GameHandler::Card;
using GameHandler::Factory::CardFactory;

using enum Card::Rank;
using enum Card::Suit;

class CardFactoryTest : public ::testing::Test {};

TEST(CardFactoryTest, CardCreationByShortNameShouldBeCorrect) {
    // Hearts
    EXPECT_EQ(Card(TWO, HEART), CardFactory::create("2H"));
    EXPECT_EQ(Card(THREE, HEART), CardFactory::create("3H"));
    EXPECT_EQ(Card(FOUR, HEART), CardFactory::create("4H"));
    EXPECT_EQ(Card(FIVE, HEART), CardFactory::create("5H"));
    EXPECT_EQ(Card(SIX, HEART), CardFactory::create("6H"));
    EXPECT_EQ(Card(SEVEN, HEART), CardFactory::create("7H"));
    EXPECT_EQ(Card(EIGHT, HEART), CardFactory::create("8H"));
    EXPECT_EQ(Card(NINE, HEART), CardFactory::create("9H"));
    EXPECT_EQ(Card(TEN, HEART), CardFactory::create("TH"));
    EXPECT_EQ(Card(JACK, HEART), CardFactory::create("JH"));
    EXPECT_EQ(Card(QUEEN, HEART), CardFactory::create("QH"));
    EXPECT_EQ(Card(KING, HEART), CardFactory::create("KH"));
    EXPECT_EQ(Card(ACE, HEART), CardFactory::create("AH"));
    // Diamonds
    EXPECT_EQ(Card(TWO, DIAMOND), CardFactory::create("2D"));
    EXPECT_EQ(Card(THREE, DIAMOND), CardFactory::create("3D"));
    EXPECT_EQ(Card(FOUR, DIAMOND), CardFactory::create("4D"));
    EXPECT_EQ(Card(FIVE, DIAMOND), CardFactory::create("5D"));
    EXPECT_EQ(Card(SIX, DIAMOND), CardFactory::create("6D"));
    EXPECT_EQ(Card(SEVEN, DIAMOND), CardFactory::create("7D"));
    EXPECT_EQ(Card(EIGHT, DIAMOND), CardFactory::create("8D"));
    EXPECT_EQ(Card(NINE, DIAMOND), CardFactory::create("9D"));
    EXPECT_EQ(Card(TEN, DIAMOND), CardFactory::create("TD"));
    EXPECT_EQ(Card(JACK, DIAMOND), CardFactory::create("JD"));
    EXPECT_EQ(Card(QUEEN, DIAMOND), CardFactory::create("QD"));
    EXPECT_EQ(Card(KING, DIAMOND), CardFactory::create("KD"));
    EXPECT_EQ(Card(ACE, DIAMOND), CardFactory::create("AD"));
    // Club
    EXPECT_EQ(Card(TWO, CLUB), CardFactory::create("2C"));
    EXPECT_EQ(Card(THREE, CLUB), CardFactory::create("3C"));
    EXPECT_EQ(Card(FOUR, CLUB), CardFactory::create("4C"));
    EXPECT_EQ(Card(FIVE, CLUB), CardFactory::create("5C"));
    EXPECT_EQ(Card(SIX, CLUB), CardFactory::create("6C"));
    EXPECT_EQ(Card(SEVEN, CLUB), CardFactory::create("7C"));
    EXPECT_EQ(Card(EIGHT, CLUB), CardFactory::create("8C"));
    EXPECT_EQ(Card(NINE, CLUB), CardFactory::create("9C"));
    EXPECT_EQ(Card(TEN, CLUB), CardFactory::create("TC"));
    EXPECT_EQ(Card(JACK, CLUB), CardFactory::create("JC"));
    EXPECT_EQ(Card(QUEEN, CLUB), CardFactory::create("QC"));
    EXPECT_EQ(Card(KING, CLUB), CardFactory::create("KC"));
    EXPECT_EQ(Card(ACE, CLUB), CardFactory::create("AC"));
    // Spade
    EXPECT_EQ(Card(TWO, SPADE), CardFactory::create("2S"));
    EXPECT_EQ(Card(THREE, SPADE), CardFactory::create("3S"));
    EXPECT_EQ(Card(FOUR, SPADE), CardFactory::create("4S"));
    EXPECT_EQ(Card(FIVE, SPADE), CardFactory::create("5S"));
    EXPECT_EQ(Card(SIX, SPADE), CardFactory::create("6S"));
    EXPECT_EQ(Card(SEVEN, SPADE), CardFactory::create("7S"));
    EXPECT_EQ(Card(EIGHT, SPADE), CardFactory::create("8S"));
    EXPECT_EQ(Card(NINE, SPADE), CardFactory::create("9S"));
    EXPECT_EQ(Card(TEN, SPADE), CardFactory::create("TS"));
    EXPECT_EQ(Card(JACK, SPADE), CardFactory::create("JS"));
    EXPECT_EQ(Card(QUEEN, SPADE), CardFactory::create("QS"));
    EXPECT_EQ(Card(KING, SPADE), CardFactory::create("KS"));
    EXPECT_EQ(Card(ACE, SPADE), CardFactory::create("AS"));
}
