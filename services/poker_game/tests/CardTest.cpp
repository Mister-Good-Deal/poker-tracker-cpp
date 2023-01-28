#include "Card.hpp"
#include "gtest/gtest.h"

TEST(CardTest, Constructor) {
    Card card(Card::Rank::ACE, Card::Suit::SPADE);

    EXPECT_EQ(Card::Suit::SPADE, card.getSuit());
    EXPECT_EQ(Card::Rank::ACE, card.getRank());
}
