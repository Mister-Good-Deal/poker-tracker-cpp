#include <gtest/gtest.h>

#include <game_handler/Card.hpp>

using GameHandler::Card;

using enum Card::Rank;
using enum Card::Suit;

class CardTest : public ::testing::Test {};

TEST(CardTest, ConstructorShouldSetRankAndSuitCorrectly) {
    Card card(ACE, SPADE);

    EXPECT_EQ(SPADE, card.getSuit());
    EXPECT_EQ(ACE, card.getRank());
}

TEST(CardTest, FullNamesShouldBeCorrect) {
    // Hearts
    EXPECT_STREQ(fmt::format("{:l}", Card(TWO, HEART)).c_str(), "Two of Heart");
    EXPECT_STREQ(fmt::format("{:l}", Card(THREE, HEART)).c_str(), "Three of Heart");
    EXPECT_STREQ(fmt::format("{:l}", Card(FOUR, HEART)).c_str(), "Four of Heart");
    EXPECT_STREQ(fmt::format("{:l}", Card(FIVE, HEART)).c_str(), "Five of Heart");
    EXPECT_STREQ(fmt::format("{:l}", Card(SIX, HEART)).c_str(), "Six of Heart");
    EXPECT_STREQ(fmt::format("{:l}", Card(SEVEN, HEART)).c_str(), "Seven of Heart");
    EXPECT_STREQ(fmt::format("{:l}", Card(EIGHT, HEART)).c_str(), "Eight of Heart");
    EXPECT_STREQ(fmt::format("{:l}", Card(NINE, HEART)).c_str(), "Nine of Heart");
    EXPECT_STREQ(fmt::format("{:l}", Card(TEN, HEART)).c_str(), "Ten of Heart");
    EXPECT_STREQ(fmt::format("{:l}", Card(JACK, HEART)).c_str(), "Jack of Heart");
    EXPECT_STREQ(fmt::format("{:l}", Card(QUEEN, HEART)).c_str(), "Queen of Heart");
    EXPECT_STREQ(fmt::format("{:l}", Card(KING, HEART)).c_str(), "King of Heart");
    EXPECT_STREQ(fmt::format("{:l}", Card(ACE, HEART)).c_str(), "Ace of Heart");
    // Diamonds
    EXPECT_STREQ(fmt::format("{:l}", Card(TWO, DIAMOND)).c_str(), "Two of Diamond");
    EXPECT_STREQ(fmt::format("{:l}", Card(THREE, DIAMOND)).c_str(), "Three of Diamond");
    EXPECT_STREQ(fmt::format("{:l}", Card(FOUR, DIAMOND)).c_str(), "Four of Diamond");
    EXPECT_STREQ(fmt::format("{:l}", Card(FIVE, DIAMOND)).c_str(), "Five of Diamond");
    EXPECT_STREQ(fmt::format("{:l}", Card(SIX, DIAMOND)).c_str(), "Six of Diamond");
    EXPECT_STREQ(fmt::format("{:l}", Card(SEVEN, DIAMOND)).c_str(), "Seven of Diamond");
    EXPECT_STREQ(fmt::format("{:l}", Card(EIGHT, DIAMOND)).c_str(), "Eight of Diamond");
    EXPECT_STREQ(fmt::format("{:l}", Card(NINE, DIAMOND)).c_str(), "Nine of Diamond");
    EXPECT_STREQ(fmt::format("{:l}", Card(TEN, DIAMOND)).c_str(), "Ten of Diamond");
    EXPECT_STREQ(fmt::format("{:l}", Card(JACK, DIAMOND)).c_str(), "Jack of Diamond");
    EXPECT_STREQ(fmt::format("{:l}", Card(QUEEN, DIAMOND)).c_str(), "Queen of Diamond");
    EXPECT_STREQ(fmt::format("{:l}", Card(KING, DIAMOND)).c_str(), "King of Diamond");
    EXPECT_STREQ(fmt::format("{:l}", Card(ACE, DIAMOND)).c_str(), "Ace of Diamond");
    // Club
    EXPECT_STREQ(fmt::format("{:l}", Card(TWO, CLUB)).c_str(), "Two of Club");
    EXPECT_STREQ(fmt::format("{:l}", Card(THREE, CLUB)).c_str(), "Three of Club");
    EXPECT_STREQ(fmt::format("{:l}", Card(FOUR, CLUB)).c_str(), "Four of Club");
    EXPECT_STREQ(fmt::format("{:l}", Card(FIVE, CLUB)).c_str(), "Five of Club");
    EXPECT_STREQ(fmt::format("{:l}", Card(SIX, CLUB)).c_str(), "Six of Club");
    EXPECT_STREQ(fmt::format("{:l}", Card(SEVEN, CLUB)).c_str(), "Seven of Club");
    EXPECT_STREQ(fmt::format("{:l}", Card(EIGHT, CLUB)).c_str(), "Eight of Club");
    EXPECT_STREQ(fmt::format("{:l}", Card(NINE, CLUB)).c_str(), "Nine of Club");
    EXPECT_STREQ(fmt::format("{:l}", Card(TEN, CLUB)).c_str(), "Ten of Club");
    EXPECT_STREQ(fmt::format("{:l}", Card(JACK, CLUB)).c_str(), "Jack of Club");
    EXPECT_STREQ(fmt::format("{:l}", Card(QUEEN, CLUB)).c_str(), "Queen of Club");
    EXPECT_STREQ(fmt::format("{:l}", Card(KING, CLUB)).c_str(), "King of Club");
    EXPECT_STREQ(fmt::format("{:l}", Card(ACE, CLUB)).c_str(), "Ace of Club");
    // Spade
    EXPECT_STREQ(fmt::format("{:l}", Card(TWO, SPADE)).c_str(), "Two of Spade");
    EXPECT_STREQ(fmt::format("{:l}", Card(THREE, SPADE)).c_str(), "Three of Spade");
    EXPECT_STREQ(fmt::format("{:l}", Card(FOUR, SPADE)).c_str(), "Four of Spade");
    EXPECT_STREQ(fmt::format("{:l}", Card(FIVE, SPADE)).c_str(), "Five of Spade");
    EXPECT_STREQ(fmt::format("{:l}", Card(SIX, SPADE)).c_str(), "Six of Spade");
    EXPECT_STREQ(fmt::format("{:l}", Card(SEVEN, SPADE)).c_str(), "Seven of Spade");
    EXPECT_STREQ(fmt::format("{:l}", Card(EIGHT, SPADE)).c_str(), "Eight of Spade");
    EXPECT_STREQ(fmt::format("{:l}", Card(NINE, SPADE)).c_str(), "Nine of Spade");
    EXPECT_STREQ(fmt::format("{:l}", Card(TEN, SPADE)).c_str(), "Ten of Spade");
    EXPECT_STREQ(fmt::format("{:l}", Card(JACK, SPADE)).c_str(), "Jack of Spade");
    EXPECT_STREQ(fmt::format("{:l}", Card(QUEEN, SPADE)).c_str(), "Queen of Spade");
    EXPECT_STREQ(fmt::format("{:l}", Card(KING, SPADE)).c_str(), "King of Spade");
    EXPECT_STREQ(fmt::format("{:l}", Card(ACE, SPADE)).c_str(), "Ace of Spade");
}

TEST(CardTest, ShortNamesShouldBeCorrect) {
    // Hearts
    EXPECT_STREQ(fmt::format("{:s}", Card(TWO, HEART)).c_str(), "2H");
    EXPECT_STREQ(fmt::format("{:s}", Card(THREE, HEART)).c_str(), "3H");
    EXPECT_STREQ(fmt::format("{:s}", Card(FOUR, HEART)).c_str(), "4H");
    EXPECT_STREQ(fmt::format("{:s}", Card(FIVE, HEART)).c_str(), "5H");
    EXPECT_STREQ(fmt::format("{:s}", Card(SIX, HEART)).c_str(), "6H");
    EXPECT_STREQ(fmt::format("{:s}", Card(SEVEN, HEART)).c_str(), "7H");
    EXPECT_STREQ(fmt::format("{:s}", Card(EIGHT, HEART)).c_str(), "8H");
    EXPECT_STREQ(fmt::format("{:s}", Card(NINE, HEART)).c_str(), "9H");
    EXPECT_STREQ(fmt::format("{:s}", Card(TEN, HEART)).c_str(), "TH");
    EXPECT_STREQ(fmt::format("{:s}", Card(JACK, HEART)).c_str(), "JH");
    EXPECT_STREQ(fmt::format("{:s}", Card(QUEEN, HEART)).c_str(), "QH");
    EXPECT_STREQ(fmt::format("{:s}", Card(KING, HEART)).c_str(), "KH");
    EXPECT_STREQ(fmt::format("{:s}", Card(ACE, HEART)).c_str(), "AH");
    // Diamonds
    EXPECT_STREQ(fmt::format("{:s}", Card(TWO, DIAMOND)).c_str(), "2D");
    EXPECT_STREQ(fmt::format("{:s}", Card(THREE, DIAMOND)).c_str(), "3D");
    EXPECT_STREQ(fmt::format("{:s}", Card(FOUR, DIAMOND)).c_str(), "4D");
    EXPECT_STREQ(fmt::format("{:s}", Card(FIVE, DIAMOND)).c_str(), "5D");
    EXPECT_STREQ(fmt::format("{:s}", Card(SIX, DIAMOND)).c_str(), "6D");
    EXPECT_STREQ(fmt::format("{:s}", Card(SEVEN, DIAMOND)).c_str(), "7D");
    EXPECT_STREQ(fmt::format("{:s}", Card(EIGHT, DIAMOND)).c_str(), "8D");
    EXPECT_STREQ(fmt::format("{:s}", Card(NINE, DIAMOND)).c_str(), "9D");
    EXPECT_STREQ(fmt::format("{:s}", Card(TEN, DIAMOND)).c_str(), "TD");
    EXPECT_STREQ(fmt::format("{:s}", Card(JACK, DIAMOND)).c_str(), "JD");
    EXPECT_STREQ(fmt::format("{:s}", Card(QUEEN, DIAMOND)).c_str(), "QD");
    EXPECT_STREQ(fmt::format("{:s}", Card(KING, DIAMOND)).c_str(), "KD");
    EXPECT_STREQ(fmt::format("{:s}", Card(ACE, DIAMOND)).c_str(), "AD");
    // Club
    EXPECT_STREQ(fmt::format("{:s}", Card(TWO, CLUB)).c_str(), "2C");
    EXPECT_STREQ(fmt::format("{:s}", Card(THREE, CLUB)).c_str(), "3C");
    EXPECT_STREQ(fmt::format("{:s}", Card(FOUR, CLUB)).c_str(), "4C");
    EXPECT_STREQ(fmt::format("{:s}", Card(FIVE, CLUB)).c_str(), "5C");
    EXPECT_STREQ(fmt::format("{:s}", Card(SIX, CLUB)).c_str(), "6C");
    EXPECT_STREQ(fmt::format("{:s}", Card(SEVEN, CLUB)).c_str(), "7C");
    EXPECT_STREQ(fmt::format("{:s}", Card(EIGHT, CLUB)).c_str(), "8C");
    EXPECT_STREQ(fmt::format("{:s}", Card(NINE, CLUB)).c_str(), "9C");
    EXPECT_STREQ(fmt::format("{:s}", Card(TEN, CLUB)).c_str(), "TC");
    EXPECT_STREQ(fmt::format("{:s}", Card(JACK, CLUB)).c_str(), "JC");
    EXPECT_STREQ(fmt::format("{:s}", Card(QUEEN, CLUB)).c_str(), "QC");
    EXPECT_STREQ(fmt::format("{:s}", Card(KING, CLUB)).c_str(), "KC");
    EXPECT_STREQ(fmt::format("{:s}", Card(ACE, CLUB)).c_str(), "AC");
    // Spade
    EXPECT_STREQ(fmt::format("{:s}", Card(TWO, SPADE)).c_str(), "2S");
    EXPECT_STREQ(fmt::format("{:s}", Card(THREE, SPADE)).c_str(), "3S");
    EXPECT_STREQ(fmt::format("{:s}", Card(FOUR, SPADE)).c_str(), "4S");
    EXPECT_STREQ(fmt::format("{:s}", Card(FIVE, SPADE)).c_str(), "5S");
    EXPECT_STREQ(fmt::format("{:s}", Card(SIX, SPADE)).c_str(), "6S");
    EXPECT_STREQ(fmt::format("{:s}", Card(SEVEN, SPADE)).c_str(), "7S");
    EXPECT_STREQ(fmt::format("{:s}", Card(EIGHT, SPADE)).c_str(), "8S");
    EXPECT_STREQ(fmt::format("{:s}", Card(NINE, SPADE)).c_str(), "9S");
    EXPECT_STREQ(fmt::format("{:s}", Card(TEN, SPADE)).c_str(), "TS");
    EXPECT_STREQ(fmt::format("{:s}", Card(JACK, SPADE)).c_str(), "JS");
    EXPECT_STREQ(fmt::format("{:s}", Card(QUEEN, SPADE)).c_str(), "QS");
    EXPECT_STREQ(fmt::format("{:s}", Card(KING, SPADE)).c_str(), "KS");
    EXPECT_STREQ(fmt::format("{:s}", Card(ACE, SPADE)).c_str(), "AS");
}

TEST(CardTest, JsonRepresentationShouldBeCorrent) {
    // language=json
    auto expectedJson = R"(
        {
            "shortName": "TS",
            "rank": "Ten",
            "suit": "Spade"
        }
    )"_json;

    EXPECT_EQ(Card(TEN, SPADE).toJson(), expectedJson);
}
