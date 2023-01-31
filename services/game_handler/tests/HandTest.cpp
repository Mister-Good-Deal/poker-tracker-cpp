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

using GameHandler::Card;
using GameHandler::Hand;
using GameHandler::Factory::CardFactory;

const std::function<Card(const std::string&)> card = CardFactory::create;

class MockHand : public Hand {
    public:
        MockHand(const Card& firstCard, const Card& secondCard) : Hand(firstCard, secondCard) {}
        ~MockHand() override = default;
        // Proxy
        auto isSuitedProxy() -> bool { return isSuited(); }
        auto isBroadwayProxy() -> bool { return isBroadway(); }
        auto isPlurProxy() -> bool { return isPlur(); }
        auto isConnectedProxy() -> bool { return isConnected(); }
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

TEST(HandTest, isSuitedProxyShouldbeCorrect) {
    EXPECT_TRUE(MockHand(card("AS"), card("TS")).isSuitedProxy());
    EXPECT_TRUE(MockHand(card("8D"), card("KD")).isSuitedProxy());
    EXPECT_TRUE(MockHand(card("2H"), card("3H")).isSuitedProxy());
    EXPECT_TRUE(MockHand(card("7C"), card("4C")).isSuitedProxy());

    EXPECT_FALSE(MockHand(card("AS"), card("TD")).isSuitedProxy());
    EXPECT_FALSE(MockHand(card("KD"), card("KS")).isSuitedProxy());
    EXPECT_FALSE(MockHand(card("2H"), card("3C")).isSuitedProxy());
    EXPECT_FALSE(MockHand(card("7C"), card("7H")).isSuitedProxy());
}

TEST(HandTest, isBroadwayShouldbeCorrect) {
    EXPECT_TRUE(MockHand(card("AS"), card("7S")).isBroadwayProxy());
    EXPECT_TRUE(MockHand(card("8D"), card("TD")).isBroadwayProxy());
    EXPECT_TRUE(MockHand(card("2H"), card("KS")).isBroadwayProxy());
    EXPECT_TRUE(MockHand(card("QC"), card("2H")).isBroadwayProxy());

    EXPECT_FALSE(MockHand(card("2S"), card("9D")).isBroadwayProxy());
    EXPECT_FALSE(MockHand(card("7D"), card("8S")).isBroadwayProxy());
    EXPECT_FALSE(MockHand(card("2H"), card("3C")).isBroadwayProxy());
    EXPECT_FALSE(MockHand(card("7C"), card("7H")).isBroadwayProxy());
}

TEST(HandTest, isPlurShouldbeCorrect) {
    EXPECT_TRUE(MockHand(card("AS"), card("TS")).isPlurProxy());
    EXPECT_TRUE(MockHand(card("QD"), card("TD")).isPlurProxy());
    EXPECT_TRUE(MockHand(card("KH"), card("KS")).isPlurProxy());
    EXPECT_TRUE(MockHand(card("QC"), card("JH")).isPlurProxy());

    EXPECT_FALSE(MockHand(card("JS"), card("9D")).isPlurProxy());
    EXPECT_FALSE(MockHand(card("7D"), card("8S")).isPlurProxy());
    EXPECT_FALSE(MockHand(card("AH"), card("9C")).isPlurProxy());
    EXPECT_FALSE(MockHand(card("7C"), card("7H")).isPlurProxy());
}

TEST(HandTest, isConnectedShouldbeCorrect) {
    EXPECT_TRUE(MockHand(card("AS"), card("KS")).isConnectedProxy());
    EXPECT_TRUE(MockHand(card("7D"), card("8D")).isConnectedProxy());
    EXPECT_TRUE(MockHand(card("9H"), card("TS")).isConnectedProxy());
    EXPECT_TRUE(MockHand(card("AC"), card("2H")).isConnectedProxy());
    EXPECT_TRUE(MockHand(card("2C"), card("AC")).isConnectedProxy());

    EXPECT_FALSE(MockHand(card("JS"), card("9D")).isConnectedProxy());
    EXPECT_FALSE(MockHand(card("7D"), card("9S")).isConnectedProxy());
    EXPECT_FALSE(MockHand(card("AH"), card("3C")).isConnectedProxy());
    EXPECT_FALSE(MockHand(card("3S"), card("AH")).isConnectedProxy());
}
