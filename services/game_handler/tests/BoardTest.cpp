#include <gtest/gtest.h>

#include <game_handler/Board.hpp>
#include <game_handler/CardFactory.hpp>

using GameHandler::Board;
using GameHandler::Factory::card;

using enum GameHandler::HandRank;

class BoardTest : public ::testing::Test {};

// --------------------- Board check ---------------------//

TEST(BoardTest, hasPossibleStraightShouldBeCorrect) {
    EXPECT_FALSE(Board({card("2S"), card("3H"), card("7D")}).hasPossibleStraight());
    EXPECT_FALSE(Board({card("7S"), card("8H"), card("QD"), card("KD"), card("KS")}).hasPossibleStraight());
    EXPECT_TRUE(Board({card("7S"), card("8H"), card("QD"), card("KD"), card("AS")}).hasPossibleStraight());
    EXPECT_TRUE(Board({card("2S"), card("3H"), card("QD"), card("KD"), card("AS")}).hasPossibleStraight());
}

TEST(BoardTest, hasPossibleFlushDrawShouldBeCorrect) {
    EXPECT_FALSE(Board({card("2S"), card("3H"), card("7D")}).hasPossibleFlushDraw());
    EXPECT_FALSE(Board({card("2S"), card("3H"), card("3D")}).hasPossibleFlushDraw());
    EXPECT_TRUE(Board({card("AS"), card("AH"), card("KS")}).hasPossibleFlushDraw());
    EXPECT_TRUE(Board({card("2S"), card("3S"), card("3H"), card("6H")}).hasPossibleFlushDraw());
}

TEST(BoardTest, hasPossibleFlushShouldBeCorrect) {
    EXPECT_FALSE(Board({card("2S"), card("3H"), card("7D")}).hasPossibleFlush());
    EXPECT_FALSE(Board({card("2S"), card("2H"), card("7D"), card("7D")}).hasPossibleFlush());
    EXPECT_TRUE(Board({card("2S"), card("2H"), card("7D"), card("7D"), card("8D")}).hasPossibleFlush());
    EXPECT_TRUE(Board({card("2S"), card("2H"), card("9S"), card("7D"), card("AS")}).hasPossibleFlush());
}

TEST(BoardTest, hasPairShouldBeCorrect) {
    EXPECT_FALSE(Board({card("2S"), card("3H"), card("7D")}).hasPair());
    EXPECT_FALSE(Board({card("2S"), card("AH"), card("7D"), card("8D"), card("9D")}).hasPair());
    EXPECT_TRUE(Board({card("2S"), card("2H"), card("7D"), card("7D"), card("8D")}).hasPair());
    EXPECT_TRUE(Board({card("2S"), card("AH"), card("9S"), card("7D"), card("AS")}).hasPair());
}

TEST(BoardTest, hasTwoPairShouldBeCorrect) {
    EXPECT_FALSE(Board({card("2S"), card("3H"), card("7D")}).hasTwoPair());
    EXPECT_FALSE(Board({card("2S"), card("AH"), card("7D"), card("7D"), card("9D")}).hasTwoPair());
    EXPECT_TRUE(Board({card("2S"), card("2H"), card("7D"), card("7D"), card("8D")}).hasTwoPair());
    EXPECT_TRUE(Board({card("AS"), card("7H"), card("7S"), card("8D"), card("AH")}).hasTwoPair());
}

TEST(BoardTest, hasTripsShouldBeCorrect) {
    EXPECT_FALSE(Board({card("2S"), card("3H"), card("7D")}).hasTrips());
    EXPECT_FALSE(Board({card("AS"), card("AH"), card("7D"), card("7D"), card("9D")}).hasTrips());
    EXPECT_TRUE(Board({card("2S"), card("2H"), card("7D"), card("7D"), card("2D")}).hasTrips());
    EXPECT_TRUE(Board({card("AS"), card("7H"), card("7S"), card("7D"), card("AH")}).hasTrips());
}

TEST(BoardTest, hasStraightShouldBeCorrect) {
    EXPECT_FALSE(Board({card("2S"), card("3H"), card("7D")}).hasStraight());
    EXPECT_FALSE(Board({card("7S"), card("8H"), card("9D"), card("6D"), card("4D")}).hasStraight());
    EXPECT_TRUE(Board({card("AS"), card("2H"), card("3D"), card("4D"), card("5D")}).hasStraight());
    EXPECT_TRUE(Board({card("AS"), card("QH"), card("JS"), card("TD"), card("KH")}).hasStraight());
}

TEST(BoardTest, hasFlushShouldBeCorrect) {
    EXPECT_FALSE(Board({card("2S"), card("3H"), card("7D")}).hasFlush());
    EXPECT_FALSE(Board({card("7S"), card("8S"), card("9S"), card("6S")}).hasFlush());
    EXPECT_TRUE(Board({card("AS"), card("KS"), card("3S"), card("4S"), card("5S")}).hasFlush());
    EXPECT_TRUE(Board({card("AH"), card("QH"), card("JH"), card("TH"), card("KH")}).hasFlush());
}

TEST(BoardTest, hasFullShouldBeCorrect) {
    EXPECT_FALSE(Board({card("2S"), card("3H"), card("7D")}).hasFull());
    EXPECT_FALSE(Board({card("7S"), card("7H"), card("7D"), card("6S")}).hasFull());
    EXPECT_TRUE(Board({card("AH"), card("AS"), card("AD"), card("2C"), card("2S")}).hasFull());
    EXPECT_TRUE(Board({card("KH"), card("3S"), card("3D"), card("3C"), card("KD")}).hasFull());
}

TEST(BoardTest, hasQuadsShouldBeCorrect) {
    EXPECT_FALSE(Board({card("2S"), card("3H"), card("7D")}).hasQuads());
    EXPECT_FALSE(Board({card("7S"), card("7H"), card("7D"), card("6S")}).hasQuads());
    EXPECT_TRUE(Board({card("AH"), card("AS"), card("AD"), card("AC"), card("2S")}).hasQuads());
    EXPECT_TRUE(Board({card("KH"), card("3S"), card("3D"), card("3C"), card("3H")}).hasQuads());
}

TEST(BoardTest, hasStraightFlushShouldBeCorrect) {
    EXPECT_FALSE(Board({card("2S"), card("3H"), card("7D")}).hasStraightFlush());
    EXPECT_FALSE(Board({card("7S"), card("8S"), card("9S"), card("6S")}).hasStraightFlush());
    EXPECT_TRUE(Board({card("AS"), card("2S"), card("3S"), card("4S"), card("5S")}).hasStraightFlush());
    EXPECT_TRUE(Board({card("AH"), card("QH"), card("JH"), card("TH"), card("KH")}).hasStraightFlush());
}

// --------------------- Hand check ---------------------//

TEST(BoardTest, handRankHighCardShouldBeCorrect) {
    EXPECT_EQ(Board({card("2S"), card("3H"), card("7D")}).getHandRank({card("9D"), card("4D")}), HIGH_CARD);
    EXPECT_EQ(Board({card("2S"), card("3H"), card("7D"), card("8D")}).getHandRank({card("9D"), card("TD")}), HIGH_CARD);
    EXPECT_EQ(Board({card("2S"), card("3H"), card("6D"), card("7H")}).getHandRank({card("9D"), card("TD")}), HIGH_CARD);
    EXPECT_EQ(Board({card("2S"), card("4H"), card("7D"), card("3S"), card("8D")}).getHandRank({card("KD"), card("JD")}), HIGH_CARD);
}

TEST(BoardTest, handRankPairShouldBeCorrect) {
    EXPECT_EQ(Board({card("2S"), card("3H"), card("7D")}).getHandRank({card("9D"), card("3D")}), PAIR);
    EXPECT_EQ(Board({card("2S"), card("3H"), card("7D"), card("8D")}).getHandRank({card("9D"), card("8D")}), PAIR);
    EXPECT_EQ(Board({card("2S"), card("3H"), card("7D"), card("7H")}).getHandRank({card("9D"), card("TD")}), PAIR);
    EXPECT_EQ(Board({card("2S"), card("3H"), card("7D"), card("3S"), card("8D")}).getHandRank({card("KD"), card("JD")}), PAIR);
}

TEST(BoardTest, handRankTwoPairShouldBeCorrect) {
    EXPECT_EQ(Board({card("2S"), card("3H"), card("7D")}).getHandRank({card("2D"), card("7C")}), TWO_PAIR);
    EXPECT_EQ(Board({card("2S"), card("3H"), card("7D"), card("8D")}).getHandRank({card("2D"), card("8D")}), TWO_PAIR);
    EXPECT_EQ(Board({card("2S"), card("3H"), card("7D"), card("7H")}).getHandRank({card("9D"), card("9S")}), TWO_PAIR);
    EXPECT_EQ(Board({card("2S"), card("3H"), card("7D"), card("3S"), card("8D")}).getHandRank({card("KD"), card("2D")}), TWO_PAIR);
}

TEST(BoardTest, handRankTripsShouldBeCorrect) {
    EXPECT_EQ(Board({card("2S"), card("3H"), card("7D")}).getHandRank({card("7H"), card("7C")}), TRIPS);
    EXPECT_EQ(Board({card("2S"), card("8H"), card("7D"), card("8D")}).getHandRank({card("AD"), card("8C")}), TRIPS);
    EXPECT_EQ(Board({card("2S"), card("9H"), card("7D"), card("6H")}).getHandRank({card("9D"), card("9S")}), TRIPS);
    EXPECT_EQ(Board({card("AS"), card("AH"), card("7D"), card("3S"), card("8D")}).getHandRank({card("AD"), card("2D")}), TRIPS);
}

TEST(BoardTest, handRankStraightShouldBeCorrect) {
    EXPECT_EQ(Board({card("2S"), card("3H"), card("4D")}).getHandRank({card("5H"), card("6C")}), STRAIGHT);
    EXPECT_EQ(Board({card("8S"), card("8H"), card("7D"), card("6D")}).getHandRank({card("5D"), card("9C")}), STRAIGHT);
    EXPECT_EQ(Board({card("2S"), card("8H"), card("7D"), card("6H")}).getHandRank({card("9D"), card("TS")}), STRAIGHT);
    EXPECT_EQ(Board({card("AS"), card("AH"), card("KD"), card("QS"), card("AC")}).getHandRank({card("TD"), card("JD")}), STRAIGHT);
}

TEST(BoardTest, handRankFlushShouldBeCorrect) {
    EXPECT_EQ(Board({card("2S"), card("3S"), card("4S")}).getHandRank({card("5S"), card("7S")}), FLUSH);
    EXPECT_EQ(Board({card("8S"), card("8D"), card("7D"), card("6D")}).getHandRank({card("5D"), card("TD")}), FLUSH);
    EXPECT_EQ(Board({card("2S"), card("8S"), card("7S"), card("6S")}).getHandRank({card("KD"), card("TS")}), FLUSH);
    EXPECT_EQ(Board({card("AS"), card("AD"), card("KD"), card("QS"), card("QD")}).getHandRank({card("9D"), card("JD")}), FLUSH);
}

TEST(BoardTest, handRankFullShouldBeCorrect) {
    EXPECT_EQ(Board({card("2S"), card("2H"), card("7D")}).getHandRank({card("7H"), card("7C")}), FULL);
    EXPECT_EQ(Board({card("2S"), card("8H"), card("7D"), card("8D")}).getHandRank({card("2D"), card("8C")}), FULL);
    EXPECT_EQ(Board({card("2S"), card("9H"), card("7D"), card("7H")}).getHandRank({card("9D"), card("9S")}), FULL);
    EXPECT_EQ(Board({card("AS"), card("AH"), card("7D"), card("2S"), card("8D")}).getHandRank({card("AD"), card("2D")}), FULL);
}

TEST(BoardTest, handRankQuadsShouldBeCorrect) {
    EXPECT_EQ(Board({card("7S"), card("3H"), card("7D")}).getHandRank({card("7H"), card("7C")}), QUADS);
    EXPECT_EQ(Board({card("2S"), card("8H"), card("8D"), card("8S")}).getHandRank({card("AD"), card("8C")}), QUADS);
    EXPECT_EQ(Board({card("2S"), card("9H"), card("9C"), card("6H")}).getHandRank({card("9D"), card("9S")}), QUADS);
    EXPECT_EQ(Board({card("AS"), card("AH"), card("7D"), card("3S"), card("8D")}).getHandRank({card("AD"), card("AC")}), QUADS);
}

TEST(BoardTest, handRankStraightFlushShouldBeCorrect) {
    EXPECT_EQ(Board({card("2S"), card("3S"), card("4S")}).getHandRank({card("5S"), card("6S")}), STRAIGHT_FLUSH);
    EXPECT_EQ(Board({card("8S"), card("8D"), card("7D"), card("6D")}).getHandRank({card("5D"), card("9D")}), STRAIGHT_FLUSH);
    EXPECT_EQ(Board({card("2S"), card("8S"), card("7S"), card("6S")}).getHandRank({card("9D"), card("TS")}), STRAIGHT_FLUSH);
    EXPECT_EQ(Board({card("AS"), card("AD"), card("KD"), card("QS"), card("QD")}).getHandRank({card("TD"), card("JD")}),
              STRAIGHT_FLUSH);
}

TEST(BoardTest, jsonRepresentationShouldBeCorrect) {
    // language=json
    auto expectedJson = R"(
        [
            { "shortName": "AH", "rank": "Ace", "suit": "Heart" },
            { "shortName": "QH", "rank": "Queen", "suit": "Heart" },
            { "shortName": "JH", "rank": "Jack", "suit": "Heart" },
            { "shortName": "TH", "rank": "Ten", "suit": "Heart" },
            { "shortName": "KH", "rank": "King", "suit": "Heart" }
        ]
    )"_json;

    EXPECT_EQ(Board({card("AH"), card("QH"), card("JH"), card("TH"), card("KH")}).toJson(), expectedJson);
}

TEST(BoardTest, jsonDetailedRepresentationShouldBeCorrect) {
    // language=json
    auto expectedJson = R"(
        {
            "cards": [
                { "shortName": "AH", "rank": "Ace", "suit": "Heart" },
                { "shortName": "QH", "rank": "Queen", "suit": "Heart" },
                { "shortName": "JH", "rank": "Jack", "suit": "Heart" },
                { "shortName": "TH", "rank": "Ten", "suit": "Heart" },
                { "shortName": "KH", "rank": "King", "suit": "Heart" }
            ],
            "properties": {
                "paire": false,
                "doublePaire": false,
                "trips": false,
                "straight": true,
                "possibleStraight": true,
                "flush": true,
                "possibleFlush": true,
                "possibleFlushDraw": true,
                "full": false,
                "quads": false,
                "straightFlush": true
            }
        }
    )"_json;

    EXPECT_EQ(Board({card("AH"), card("QH"), card("JH"), card("TH"), card("KH")}).toDetailedJson(), expectedJson);
}
