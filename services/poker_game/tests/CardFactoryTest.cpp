#include "Card.hpp"
#include "CardFactory.hpp"
#include "gtest/gtest.h"

TEST(CardFactoryTest, Create) {
    CardFactory factory;
    Card        card(Card::Rank::ACE, Card::Suit::SPADE);

    EXPECT_EQ(card, factory.create("AS"));
}
