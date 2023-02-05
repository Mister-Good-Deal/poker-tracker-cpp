#include <gtest/gtest.h>

#include "Board.hpp"
#include "CardFactory.hpp"

using GameHandler::Board;
using GameHandler::Factory::card;

class MockBoard : public Board {
    public:
        MockBoard() : Board(){};
        // Proxy
        auto countPossibleStraightsProxy(int8_t withCardsNumber) -> int8_t { return countPossibleStraights(withCardsNumber); };
};

class BoardTest : public ::testing::Test {};

TEST(BoardTest, countPossibleStraightsWithTwoCardsShouldBeCorrect) {
    MockBoard mockBoard;

    mockBoard.setCards({card("2S"), card("3S"), card("7S"), card("8S"), card("KS")});

    EXPECT_EQ(mockBoard.countPossibleStraightsProxy(2), 0);

    mockBoard.setCards({card("6S"), card("7S"), card("8S"), card("AS"), card("KS")});

    EXPECT_EQ(mockBoard.countPossibleStraightsProxy(2), 3);

    mockBoard.setCards({card("2S"), card("3S"), card("8S"), card("AS"), card("KS")});

    EXPECT_EQ(mockBoard.countPossibleStraightsProxy(2), 1);

    mockBoard.setCards({card("2S"), card("3S"), card("6S"), card("AS"), card("KS")});

    EXPECT_EQ(mockBoard.countPossibleStraightsProxy(2), 2);
}

TEST(BoardTest, gutShotsShouldBeCorrect) {
    Board board;

    board.setCards({card("2S"), card("3S"), card("7S"), card("8S"), card("KS")});

    EXPECT_EQ(board.getGutShots(), 0);

    board.setCards({card("2S"), card("3S"), card("4S"), card("5S"), card("KS")});

    EXPECT_EQ(board.getGutShots(), 2);

    board.setCards({card("2S"), card("3S"), card("5S"), card("6S"), card("KS")});

    EXPECT_EQ(board.getGutShots(), 1);
}

TEST(BoardTest, hasFlushDrawShouldBeCorrect) {
    Board board;

    board.setCards({card("2S"), card("3H"), card("7D")});

    EXPECT_FALSE(board.hasFlushDraw());

    board.setCards({card("AS"), card("AH"), card("KS")});

    EXPECT_TRUE(board.hasFlushDraw());

    board.setCards({card("2S"), card("3S"), card("3H"), card("6H"), card("KD")});

    EXPECT_TRUE(board.hasFlushDraw());
}

TEST(BoardTest, hasPossibleStraightShouldBeCorrect) {
    Board board;

    board.setCards({card("2S"), card("3H"), card("7D")});

    EXPECT_FALSE(board.hasPossibleStraight());

    board.setCards({card("7S"), card("8H"), card("QD"), card("KD"), card("KS")});

    EXPECT_FALSE(board.hasPossibleStraight());

    board.setCards({card("7S"), card("8H"), card("QD"), card("KD"), card("AS")});

    EXPECT_TRUE(board.hasPossibleStraight());

    board.setCards({card("2S"), card("3H"), card("QD"), card("KD"), card("AS")});

    EXPECT_TRUE(board.hasPossibleStraight());
}

TEST(BoardTest, hasPossibleFlushShouldBeCorrect) {
    Board board;

    board.setCards({card("2S"), card("3H"), card("7D")});

    EXPECT_FALSE(board.hasPossibleFlush());

    board.setCards({card("2S"), card("2H"), card("7D"), card("7D")});

    EXPECT_FALSE(board.hasPossibleFlush());

    board.setCards({card("2S"), card("2H"), card("7D"), card("7D"), card("8D")});

    EXPECT_TRUE(board.hasPossibleFlush());

    board.setCards({card("2S"), card("2H"), card("9S"), card("7D"), card("AS")});

    EXPECT_TRUE(board.hasPossibleFlush());
}

TEST(BoardTest, hasPaireShouldBeCorrect) {
    Board board;

    board.setCards({card("2S"), card("3H"), card("7D")});

    EXPECT_FALSE(board.hasPaire());

    board.setCards({card("2S"), card("AH"), card("7D"), card("8D"), card("9D")});

    EXPECT_FALSE(board.hasPaire());

    board.setCards({card("2S"), card("2H"), card("7D"), card("7D"), card("8D")});

    EXPECT_TRUE(board.hasPaire());

    board.setCards({card("2S"), card("AH"), card("9S"), card("7D"), card("AS")});

    EXPECT_TRUE(board.hasPaire());
}

TEST(BoardTest, hasDoublePaireShouldBeCorrect) {
    Board board;

    board.setCards({card("2S"), card("3H"), card("7D")});

    EXPECT_FALSE(board.hasDoublePaire());

    board.setCards({card("2S"), card("AH"), card("7D"), card("7D"), card("9D")});

    EXPECT_FALSE(board.hasDoublePaire());

    board.setCards({card("2S"), card("2H"), card("7D"), card("7D"), card("8D")});

    EXPECT_TRUE(board.hasDoublePaire());

    board.setCards({card("AS"), card("7H"), card("7S"), card("8D"), card("AH")});

    EXPECT_TRUE(board.hasDoublePaire());
}

TEST(BoardTest, hasTripsShouldBeCorrect) {
    Board board;

    board.setCards({card("2S"), card("3H"), card("7D")});

    EXPECT_FALSE(board.hasTrips());

    board.setCards({card("AS"), card("AH"), card("7D"), card("7D"), card("9D")});

    EXPECT_FALSE(board.hasTrips());

    board.setCards({card("2S"), card("2H"), card("7D"), card("7D"), card("2D")});

    EXPECT_TRUE(board.hasTrips());

    board.setCards({card("AS"), card("7H"), card("7S"), card("7D"), card("AH")});

    EXPECT_TRUE(board.hasTrips());
}

TEST(BoardTest, hasStraightShouldBeCorrect) {
    Board board;

    board.setCards({card("2S"), card("3H"), card("7D")});

    EXPECT_FALSE(board.hasStraight());

    board.setCards({card("7S"), card("8H"), card("9D"), card("6D"), card("4D")});

    EXPECT_FALSE(board.hasStraight());

    board.setCards({card("AS"), card("2H"), card("3D"), card("4D"), card("5D")});

    EXPECT_TRUE(board.hasStraight());

    board.setCards({card("AS"), card("QH"), card("JS"), card("TD"), card("KH")});

    EXPECT_TRUE(board.hasStraight());
}

TEST(BoardTest, hasFlushShouldBeCorrect) {
    Board board;

    board.setCards({card("2S"), card("3H"), card("7D")});

    EXPECT_FALSE(board.hasFlush());

    board.setCards({card("7S"), card("8S"), card("9S"), card("6S")});

    EXPECT_FALSE(board.hasFlush());

    board.setCards({card("AS"), card("KS"), card("3S"), card("4S"), card("5S")});

    EXPECT_TRUE(board.hasFlush());

    board.setCards({card("AH"), card("QH"), card("JH"), card("TH"), card("KH")});

    EXPECT_TRUE(board.hasFlush());
}

TEST(BoardTest, hasFullShouldBeCorrect) {
    Board board;

    board.setCards({card("2S"), card("3H"), card("7D")});

    EXPECT_FALSE(board.hasFull());

    board.setCards({card("7S"), card("7H"), card("7D"), card("6S")});

    EXPECT_FALSE(board.hasFull());

    board.setCards({card("AH"), card("AS"), card("AD"), card("2C"), card("2S")});

    EXPECT_TRUE(board.hasFull());

    board.setCards({card("KH"), card("3S"), card("3D"), card("3C"), card("KD")});

    EXPECT_TRUE(board.hasFull());
}

TEST(BoardTest, hasQuadsShouldBeCorrect) {
    Board board;

    board.setCards({card("2S"), card("3H"), card("7D")});

    EXPECT_FALSE(board.hasQuads());

    board.setCards({card("7S"), card("7H"), card("7D"), card("6S")});

    EXPECT_FALSE(board.hasQuads());

    board.setCards({card("AH"), card("AS"), card("AD"), card("AC"), card("2S")});

    EXPECT_TRUE(board.hasQuads());

    board.setCards({card("KH"), card("3S"), card("3D"), card("3C"), card("3H")});

    EXPECT_TRUE(board.hasQuads());
}

TEST(BoardTest, hasStraightFlushShouldBeCorrect) {
    Board board;

    board.setCards({card("2S"), card("3H"), card("7D")});

    EXPECT_FALSE(board.hasStraightFlush());

    board.setCards({card("7S"), card("8S"), card("9S"), card("6S")});

    EXPECT_FALSE(board.hasStraightFlush());

    board.setCards({card("AS"), card("2S"), card("3S"), card("4S"), card("5S")});

    EXPECT_TRUE(board.hasStraightFlush());

    board.setCards({card("AH"), card("QH"), card("JH"), card("TH"), card("KH")});

    EXPECT_TRUE(board.hasStraightFlush());
}
