#include "CardFactory.hpp"

#include "gtest/gtest.h"

TEST(CardFactoryTest, Create) {
    // Arrange
    CardFactory factory;
    Card        card(Card::Suit::Spades, Card::Rank::Ace);

    // Act
    auto created_card = factory.create(card.toString());

    // Assert
    EXPECT_EQ(card, created_card);
}
