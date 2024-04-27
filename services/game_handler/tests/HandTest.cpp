#include <gtest/gtest.h>

#include <game_handler/CardFactory.hpp>
#include <game_handler/Hand.hpp>
#include <utilities/GtestMacros.hpp>

using GameHandler::Card;
using GameHandler::Hand;
using GameHandler::invalid_hand;
using GameHandler::Factory::card;
using GameHandler::Factory::invalid_card;

class HandTest : public ::testing::Test {};

TEST(HandTest, shouldThrowInvalidHandExceptionWhenTheTwoCardsAreTheSame) {
    EXPECT_THROW_WITH_MESSAGE(Hand(card("JS"), card("JS")), invalid_hand, "The two given cards are the same (JS)");
    EXPECT_THROW_WITH_MESSAGE(Hand(card("AD"), card("AD")), invalid_hand, "The two given cards are the same (AD)");
    EXPECT_THROW_WITH_MESSAGE(Hand(card("7H"), card("7H")), invalid_hand, "The two given cards are the same (7H)");
}

TEST(HandTest, shouldThrowInvalidCardExceptionWhenOneCardDoesNotExist) {
    EXPECT_THROW_WITH_MESSAGE(Hand(card("JS"), card("SS")), invalid_card, "Invalid short card name (SS)");
    EXPECT_THROW_WITH_MESSAGE(Hand(card("BD"), card("AD")), invalid_card, "Invalid short card name (BD)");
    EXPECT_THROW_WITH_MESSAGE(Hand(card("7H"), card("10D")), invalid_card, "Invalid short card name (10D)");
    EXPECT_THROW_WITH_MESSAGE(Hand(card("3H"), card("4G")), invalid_card, "Invalid short card name (4G)");
    EXPECT_THROW_WITH_MESSAGE(Hand(card("2T"), card("4S")), invalid_card, "Invalid short card name (2T)");
}

TEST(HandTest, isSuitedShouldbeCorrect) {
    EXPECT_TRUE(Hand(card("AS"), card("TS")).isSuited());
    EXPECT_TRUE(Hand(card("8D"), card("KD")).isSuited());
    EXPECT_TRUE(Hand(card("2H"), card("3H")).isSuited());
    EXPECT_TRUE(Hand(card("7C"), card("4C")).isSuited());

    EXPECT_FALSE(Hand(card("AS"), card("TD")).isSuited());
    EXPECT_FALSE(Hand(card("KD"), card("KS")).isSuited());
    EXPECT_FALSE(Hand(card("2H"), card("3C")).isSuited());
    EXPECT_FALSE(Hand(card("7C"), card("7H")).isSuited());
}

TEST(HandTest, isAceSuitedShouldbeCorrect) {
    EXPECT_TRUE(Hand(card("AS"), card("TS")).isAceSuited());
    EXPECT_TRUE(Hand(card("8D"), card("AD")).isAceSuited());
    EXPECT_TRUE(Hand(card("2H"), card("AH")).isAceSuited());
    EXPECT_TRUE(Hand(card("AC"), card("4C")).isAceSuited());

    EXPECT_FALSE(Hand(card("AS"), card("AD")).isAceSuited());
    EXPECT_FALSE(Hand(card("AD"), card("KS")).isAceSuited());
    EXPECT_FALSE(Hand(card("AH"), card("3C")).isAceSuited());
    EXPECT_FALSE(Hand(card("7C"), card("AH")).isAceSuited());
}

TEST(HandTest, isBroadwayShouldbeCorrect) {
    EXPECT_TRUE(Hand(card("AS"), card("7S")).isBroadway());
    EXPECT_TRUE(Hand(card("8D"), card("TD")).isBroadway());
    EXPECT_TRUE(Hand(card("2H"), card("KS")).isBroadway());
    EXPECT_TRUE(Hand(card("QC"), card("2H")).isBroadway());

    EXPECT_FALSE(Hand(card("2S"), card("9D")).isBroadway());
    EXPECT_FALSE(Hand(card("7D"), card("8S")).isBroadway());
    EXPECT_FALSE(Hand(card("2H"), card("3C")).isBroadway());
    EXPECT_FALSE(Hand(card("7C"), card("7H")).isBroadway());
}

TEST(HandTest, isPlurShouldbeCorrect) {
    EXPECT_TRUE(Hand(card("AS"), card("TS")).isPlur());
    EXPECT_TRUE(Hand(card("QD"), card("TD")).isPlur());
    EXPECT_TRUE(Hand(card("KH"), card("KS")).isPlur());
    EXPECT_TRUE(Hand(card("QC"), card("JH")).isPlur());

    EXPECT_FALSE(Hand(card("JS"), card("9D")).isPlur());
    EXPECT_FALSE(Hand(card("7D"), card("8S")).isPlur());
    EXPECT_FALSE(Hand(card("AH"), card("9C")).isPlur());
    EXPECT_FALSE(Hand(card("7C"), card("7H")).isPlur());
}

TEST(HandTest, isConnectedShouldbeCorrect) {
    EXPECT_TRUE(Hand(card("AS"), card("KS")).isConnected());
    EXPECT_TRUE(Hand(card("7D"), card("8D")).isConnected());
    EXPECT_TRUE(Hand(card("9H"), card("TS")).isConnected());
    EXPECT_TRUE(Hand(card("AC"), card("2H")).isConnected());
    EXPECT_TRUE(Hand(card("2C"), card("AC")).isConnected());

    EXPECT_FALSE(Hand(card("JS"), card("9D")).isConnected());
    EXPECT_FALSE(Hand(card("7D"), card("9S")).isConnected());
    EXPECT_FALSE(Hand(card("AH"), card("3C")).isConnected());
    EXPECT_FALSE(Hand(card("3S"), card("AH")).isConnected());
}

TEST(HandTest, isPremiumShouldbeCorrect) {
    EXPECT_TRUE(Hand(card("AS"), card("KS")).isPremium());
    EXPECT_TRUE(Hand(card("QD"), card("QS")).isPremium());
    EXPECT_TRUE(Hand(card("KH"), card("AS")).isPremium());
    EXPECT_TRUE(Hand(card("KC"), card("KH")).isPremium());

    EXPECT_FALSE(Hand(card("JS"), card("9D")).isPremium());
    EXPECT_FALSE(Hand(card("AD"), card("JS")).isPremium());
    EXPECT_FALSE(Hand(card("QC"), card("KC")).isPremium());
    EXPECT_FALSE(Hand(card("3S"), card("AH")).isPremium());
}

TEST(HandTest, jsonRepresentationShouldBeCorrect) {
    // language=json
    auto expectedJson = R"(
        [
            { "shortName": "AS", "rank": "Ace", "suit": "Spade" },
            { "shortName": "KS", "rank": "King", "suit": "Spade" }
        ]
    )"_json;

    EXPECT_EQ(Hand(card("AS"), card("KS")).toJson(), expectedJson);
    EXPECT_EQ(Hand().toJson(), "[]"_json);
}

TEST(HandTest, jsonDetailedRepresentationShouldBeCorrect) {
    // language=json
    auto expectedJson = R"(
        {
            "cards": [
                { "shortName": "TS", "rank": "Ten", "suit": "Spade" },
                { "shortName": "KS", "rank": "King", "suit": "Spade" }
            ],
            "properties": {
                "suited": true,
                "aceSuited": false,
                "broadway": true,
                "plur": true,
                "connected": false,
                "premium": false
            }
        }
    )"_json;

    EXPECT_EQ(Hand(card("TS"), card("KS")).toDetailedJson(), expectedJson);
}
