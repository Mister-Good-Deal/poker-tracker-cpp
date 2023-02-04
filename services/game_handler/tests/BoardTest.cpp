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
