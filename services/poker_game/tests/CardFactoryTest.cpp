#include "CardFactory.hpp"
#include "gtest/gtest.h"

using Factory::CardFactory;

class CardFactoryTest : public ::testing::Test {};

TEST(CardFactoryTest, CardCreationByShortNameShouldBeCorrect) {
    // Hearts
    EXPECT_EQ(Card(Rank::TWO, Suit::HEART), CardFactory::create("2H"));
    EXPECT_EQ(Card(Rank::THREE, Suit::HEART), CardFactory::create("3H"));
    EXPECT_EQ(Card(Rank::FOUR, Suit::HEART), CardFactory::create("4H"));
    EXPECT_EQ(Card(Rank::FIVE, Suit::HEART), CardFactory::create("5H"));
    EXPECT_EQ(Card(Rank::SIX, Suit::HEART), CardFactory::create("6H"));
    EXPECT_EQ(Card(Rank::SEVEN, Suit::HEART), CardFactory::create("7H"));
    EXPECT_EQ(Card(Rank::EIGHT, Suit::HEART), CardFactory::create("8H"));
    EXPECT_EQ(Card(Rank::NINE, Suit::HEART), CardFactory::create("9H"));
    EXPECT_EQ(Card(Rank::TEN, Suit::HEART), CardFactory::create("TH"));
    EXPECT_EQ(Card(Rank::JACK, Suit::HEART), CardFactory::create("JH"));
    EXPECT_EQ(Card(Rank::QUEEN, Suit::HEART), CardFactory::create("QH"));
    EXPECT_EQ(Card(Rank::KING, Suit::HEART), CardFactory::create("KH"));
    EXPECT_EQ(Card(Rank::ACE, Suit::HEART), CardFactory::create("AH"));
    // Diamonds
    EXPECT_EQ(Card(Rank::TWO, Suit::DIAMOND), CardFactory::create("2D"));
    EXPECT_EQ(Card(Rank::THREE, Suit::DIAMOND), CardFactory::create("3D"));
    EXPECT_EQ(Card(Rank::FOUR, Suit::DIAMOND), CardFactory::create("4D"));
    EXPECT_EQ(Card(Rank::FIVE, Suit::DIAMOND), CardFactory::create("5D"));
    EXPECT_EQ(Card(Rank::SIX, Suit::DIAMOND), CardFactory::create("6D"));
    EXPECT_EQ(Card(Rank::SEVEN, Suit::DIAMOND), CardFactory::create("7D"));
    EXPECT_EQ(Card(Rank::EIGHT, Suit::DIAMOND), CardFactory::create("8D"));
    EXPECT_EQ(Card(Rank::NINE, Suit::DIAMOND), CardFactory::create("9D"));
    EXPECT_EQ(Card(Rank::TEN, Suit::DIAMOND), CardFactory::create("TD"));
    EXPECT_EQ(Card(Rank::JACK, Suit::DIAMOND), CardFactory::create("JD"));
    EXPECT_EQ(Card(Rank::QUEEN, Suit::DIAMOND), CardFactory::create("QD"));
    EXPECT_EQ(Card(Rank::KING, Suit::DIAMOND), CardFactory::create("KD"));
    EXPECT_EQ(Card(Rank::ACE, Suit::DIAMOND), CardFactory::create("AD"));
    // Club
    EXPECT_EQ(Card(Rank::TWO, Suit::CLUB), CardFactory::create("2C"));
    EXPECT_EQ(Card(Rank::THREE, Suit::CLUB), CardFactory::create("3C"));
    EXPECT_EQ(Card(Rank::FOUR, Suit::CLUB), CardFactory::create("4C"));
    EXPECT_EQ(Card(Rank::FIVE, Suit::CLUB), CardFactory::create("5C"));
    EXPECT_EQ(Card(Rank::SIX, Suit::CLUB), CardFactory::create("6C"));
    EXPECT_EQ(Card(Rank::SEVEN, Suit::CLUB), CardFactory::create("7C"));
    EXPECT_EQ(Card(Rank::EIGHT, Suit::CLUB), CardFactory::create("8C"));
    EXPECT_EQ(Card(Rank::NINE, Suit::CLUB), CardFactory::create("9C"));
    EXPECT_EQ(Card(Rank::TEN, Suit::CLUB), CardFactory::create("TC"));
    EXPECT_EQ(Card(Rank::JACK, Suit::CLUB), CardFactory::create("JC"));
    EXPECT_EQ(Card(Rank::QUEEN, Suit::CLUB), CardFactory::create("QC"));
    EXPECT_EQ(Card(Rank::KING, Suit::CLUB), CardFactory::create("KC"));
    EXPECT_EQ(Card(Rank::ACE, Suit::CLUB), CardFactory::create("AC"));
    // Spade
    EXPECT_EQ(Card(Rank::TWO, Suit::SPADE), CardFactory::create("2S"));
    EXPECT_EQ(Card(Rank::THREE, Suit::SPADE), CardFactory::create("3S"));
    EXPECT_EQ(Card(Rank::FOUR, Suit::SPADE), CardFactory::create("4S"));
    EXPECT_EQ(Card(Rank::FIVE, Suit::SPADE), CardFactory::create("5S"));
    EXPECT_EQ(Card(Rank::SIX, Suit::SPADE), CardFactory::create("6S"));
    EXPECT_EQ(Card(Rank::SEVEN, Suit::SPADE), CardFactory::create("7S"));
    EXPECT_EQ(Card(Rank::EIGHT, Suit::SPADE), CardFactory::create("8S"));
    EXPECT_EQ(Card(Rank::NINE, Suit::SPADE), CardFactory::create("9S"));
    EXPECT_EQ(Card(Rank::TEN, Suit::SPADE), CardFactory::create("TS"));
    EXPECT_EQ(Card(Rank::JACK, Suit::SPADE), CardFactory::create("JS"));
    EXPECT_EQ(Card(Rank::QUEEN, Suit::SPADE), CardFactory::create("QS"));
    EXPECT_EQ(Card(Rank::KING, Suit::SPADE), CardFactory::create("KS"));
    EXPECT_EQ(Card(Rank::ACE, Suit::SPADE), CardFactory::create("AS"));
}
