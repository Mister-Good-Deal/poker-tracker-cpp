#include <gtest/gtest.h>

#include <game_handler/CardFactory.hpp>

using GameHandler::Card;
using GameHandler::Factory::card;
using GameHandler::Factory::CardFactory;

using enum Card::Rank;
using enum Card::Suit;

class CardFactoryTest : public ::testing::Test {};

TEST(CardFactoryTest, CardCreationByShortNameShouldBeCorrect) {
    // Hearts
    EXPECT_EQ(Card(TWO, HEART), card("2H"));
    EXPECT_EQ(Card(THREE, HEART), card("3H"));
    EXPECT_EQ(Card(FOUR, HEART), card("4H"));
    EXPECT_EQ(Card(FIVE, HEART), card("5H"));
    EXPECT_EQ(Card(SIX, HEART), card("6H"));
    EXPECT_EQ(Card(SEVEN, HEART), card("7H"));
    EXPECT_EQ(Card(EIGHT, HEART), card("8H"));
    EXPECT_EQ(Card(NINE, HEART), card("9H"));
    EXPECT_EQ(Card(TEN, HEART), card("TH"));
    EXPECT_EQ(Card(JACK, HEART), card("JH"));
    EXPECT_EQ(Card(QUEEN, HEART), card("QH"));
    EXPECT_EQ(Card(KING, HEART), card("KH"));
    EXPECT_EQ(Card(ACE, HEART), card("AH"));
    // Diamonds
    EXPECT_EQ(Card(TWO, DIAMOND), card("2D"));
    EXPECT_EQ(Card(THREE, DIAMOND), card("3D"));
    EXPECT_EQ(Card(FOUR, DIAMOND), card("4D"));
    EXPECT_EQ(Card(FIVE, DIAMOND), card("5D"));
    EXPECT_EQ(Card(SIX, DIAMOND), card("6D"));
    EXPECT_EQ(Card(SEVEN, DIAMOND), card("7D"));
    EXPECT_EQ(Card(EIGHT, DIAMOND), card("8D"));
    EXPECT_EQ(Card(NINE, DIAMOND), card("9D"));
    EXPECT_EQ(Card(TEN, DIAMOND), card("TD"));
    EXPECT_EQ(Card(JACK, DIAMOND), card("JD"));
    EXPECT_EQ(Card(QUEEN, DIAMOND), card("QD"));
    EXPECT_EQ(Card(KING, DIAMOND), card("KD"));
    EXPECT_EQ(Card(ACE, DIAMOND), card("AD"));
    // Club
    EXPECT_EQ(Card(TWO, CLUB), card("2C"));
    EXPECT_EQ(Card(THREE, CLUB), card("3C"));
    EXPECT_EQ(Card(FOUR, CLUB), card("4C"));
    EXPECT_EQ(Card(FIVE, CLUB), card("5C"));
    EXPECT_EQ(Card(SIX, CLUB), card("6C"));
    EXPECT_EQ(Card(SEVEN, CLUB), card("7C"));
    EXPECT_EQ(Card(EIGHT, CLUB), card("8C"));
    EXPECT_EQ(Card(NINE, CLUB), card("9C"));
    EXPECT_EQ(Card(TEN, CLUB), card("TC"));
    EXPECT_EQ(Card(JACK, CLUB), card("JC"));
    EXPECT_EQ(Card(QUEEN, CLUB), card("QC"));
    EXPECT_EQ(Card(KING, CLUB), card("KC"));
    EXPECT_EQ(Card(ACE, CLUB), card("AC"));
    // Spade
    EXPECT_EQ(Card(TWO, SPADE), card("2S"));
    EXPECT_EQ(Card(THREE, SPADE), card("3S"));
    EXPECT_EQ(Card(FOUR, SPADE), card("4S"));
    EXPECT_EQ(Card(FIVE, SPADE), card("5S"));
    EXPECT_EQ(Card(SIX, SPADE), card("6S"));
    EXPECT_EQ(Card(SEVEN, SPADE), card("7S"));
    EXPECT_EQ(Card(EIGHT, SPADE), card("8S"));
    EXPECT_EQ(Card(NINE, SPADE), card("9S"));
    EXPECT_EQ(Card(TEN, SPADE), card("TS"));
    EXPECT_EQ(Card(JACK, SPADE), card("JS"));
    EXPECT_EQ(Card(QUEEN, SPADE), card("QS"));
    EXPECT_EQ(Card(KING, SPADE), card("KS"));
    EXPECT_EQ(Card(ACE, SPADE), card("AS"));
}
