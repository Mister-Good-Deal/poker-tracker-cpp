#include "Card.hpp"
#include "gtest/gtest.h"

TEST(CardTest, Constructor) {
    // Arrange
    Card card(Card::Suit::Spades, Card::Rank::Ace);

    // Act
    Card::Suit suit = card.getSuit();
    Card::Rank rank = card.getRank();

    // Assert
    EXPECT_EQ(Card::Suit::Spades, suit);
    EXPECT_EQ(Card::Rank::Ace, rank);
}
