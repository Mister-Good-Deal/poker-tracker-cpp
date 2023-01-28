#include "CardFactory.hpp"
#include "Hand.hpp"
#include "gtest/gtest.h"

#define EXPECT_THROW_WITH_MESSAGE(code, exception_type, expected_message) \
    do {                                                                  \
        EXPECT_THROW(                                                     \
            try { code; } catch (const exception_type& e) {               \
                EXPECT_STREQ(e.what(), expected_message);                 \
                throw;                                                    \
            },                                                            \
            exception_type);                                              \
    } while (0)

using Factory::CardFactory;

using Rank = Card::Rank;
using Suit = Card::Suit;

std::function<Card(const std::string&)> card = CardFactory::create;

class MockHand : public Hand {
    public:
        MockHand(const Card& firstCard, const Card& secondCard) : Hand(firstCard, secondCard) {}
        // Proxy
        bool isSuited() override { return (*this).Hand::isSuited(); }
        bool isBroadway() override { return (*this).Hand::isBroadway(); }
        bool isPlur() override { return (*this).Hand::isPlur(); }
        bool isConnected() override { return (*this).Hand::isConnected(); }
};

class HandTest : public ::testing::Test {};

TEST(HandTest, shouldThrowInvalidHandExceptionWhenTheTwoCardsAreTheSame) {
    EXPECT_THROW_WITH_MESSAGE(Hand(card("JS"), card("JS")), Hand::invalid_hand, "The two given cards are the same (JS)");
    EXPECT_THROW_WITH_MESSAGE(Hand(card("AD"), card("AD")), Hand::invalid_hand, "The two given cards are the same (AD)");
    EXPECT_THROW_WITH_MESSAGE(Hand(card("7H"), card("7H")), Hand::invalid_hand, "The two given cards are the same (7H)");
}

TEST(HandTest, shouldThrowInvalidCardExceptionWhenOneCardDoesNotExist) {
    EXPECT_THROW_WITH_MESSAGE(Hand(card("JS"), card("SS")), CardFactory::invalid_card, "Invalid short card name (SS)");
    EXPECT_THROW_WITH_MESSAGE(Hand(card("BD"), card("AD")), CardFactory::invalid_card, "Invalid short card name (BD)");
    EXPECT_THROW_WITH_MESSAGE(Hand(card("7H"), card("10D")), CardFactory::invalid_card, "Invalid short card name (10D)");
    EXPECT_THROW_WITH_MESSAGE(Hand(card("3H"), card("4G")), CardFactory::invalid_card, "Invalid short card name (4G)");
    EXPECT_THROW_WITH_MESSAGE(Hand(card("2T"), card("4S")), CardFactory::invalid_card, "Invalid short card name (2T)");
}

TEST(HandTest, isSuitedShouldbeCorrect) {
    EXPECT_TRUE(MockHand(card("AS"), card("TS")).isSuited());
    EXPECT_TRUE(MockHand(card("8D"), card("KD")).isSuited());
    EXPECT_TRUE(MockHand(card("2H"), card("3H")).isSuited());
    EXPECT_TRUE(MockHand(card("7C"), card("4C")).isSuited());

    EXPECT_FALSE(MockHand(card("AS"), card("TD")).isSuited());
    EXPECT_FALSE(MockHand(card("KD"), card("KS")).isSuited());
    EXPECT_FALSE(MockHand(card("2H"), card("3C")).isSuited());
    EXPECT_FALSE(MockHand(card("7C"), card("7H")).isSuited());
}

TEST(HandTest, isBroadwayShouldbeCorrect) {
    EXPECT_TRUE(MockHand(card("AS"), card("7S")).isBroadway());
    EXPECT_TRUE(MockHand(card("8D"), card("TD")).isBroadway());
    EXPECT_TRUE(MockHand(card("2H"), card("KS")).isBroadway());
    EXPECT_TRUE(MockHand(card("QC"), card("2H")).isBroadway());

    EXPECT_FALSE(MockHand(card("2S"), card("9D")).isBroadway());
    EXPECT_FALSE(MockHand(card("7D"), card("8S")).isBroadway());
    EXPECT_FALSE(MockHand(card("2H"), card("3C")).isBroadway());
    EXPECT_FALSE(MockHand(card("7C"), card("7H")).isBroadway());
}

TEST(HandTest, isPlurShouldbeCorrect) {
    EXPECT_TRUE(MockHand(card("AS"), card("TS")).isPlur());
    EXPECT_TRUE(MockHand(card("QD"), card("TD")).isPlur());
    EXPECT_TRUE(MockHand(card("KH"), card("KS")).isPlur());
    EXPECT_TRUE(MockHand(card("QC"), card("JH")).isPlur());

    EXPECT_FALSE(MockHand(card("JS"), card("9D")).isPlur());
    EXPECT_FALSE(MockHand(card("7D"), card("8S")).isPlur());
    EXPECT_FALSE(MockHand(card("AH"), card("9C")).isPlur());
    EXPECT_FALSE(MockHand(card("7C"), card("7H")).isPlur());
}

TEST(HandTest, isConnectedShouldbeCorrect) {
    EXPECT_TRUE(MockHand(card("AS"), card("KS")).isConnected());
    EXPECT_TRUE(MockHand(card("7D"), card("8D")).isConnected());
    EXPECT_TRUE(MockHand(card("9H"), card("TS")).isConnected());
    EXPECT_TRUE(MockHand(card("AC"), card("2H")).isConnected());
    EXPECT_TRUE(MockHand(card("2C"), card("AC")).isConnected());

    EXPECT_FALSE(MockHand(card("JS"), card("9D")).isConnected());
    EXPECT_FALSE(MockHand(card("7D"), card("9S")).isConnected());
    EXPECT_FALSE(MockHand(card("AH"), card("3C")).isConnected());
    EXPECT_FALSE(MockHand(card("3S"), card("AH")).isConnected());
}
