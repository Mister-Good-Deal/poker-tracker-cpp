#include <gtest/gtest.h>

#include <opencv4/opencv2/imgcodecs.hpp>

#include <CardFactory.hpp>
#include <WinamaxOcr.hpp>

using GameHandler::Factory::card;
using OCR::WinamaxOcr;

using enum GameHandler::Card::Suit;
using enum GameHandler::Card::Rank;

/**
 * Avoid to load the OCR model for each test, it takes a lot of time (~1s).
 */
class Env : public ::testing::Environment {
    public:
        static auto winamaxOcr() -> const WinamaxOcr {
            static const WinamaxOcr ocr;

            return ocr;
        };
};

class WinamaxOcrTest : public ::testing::Test {};

TEST(WinamaxOcrTest, readCardRankShouldWork) {
    auto two   = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/ranks/2.png");
    auto four  = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/ranks/4.png");
    auto five  = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/ranks/5.png");
    auto seven = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/ranks/7.png");
    auto eight = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/ranks/8.png");
    auto queen = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/ranks/Q.png");
    auto king  = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/ranks/K.png");

    EXPECT_EQ(Env::winamaxOcr().readCardRank(two), TWO);
    EXPECT_EQ(Env::winamaxOcr().readCardRank(four), FOUR);
    EXPECT_EQ(Env::winamaxOcr().readCardRank(five), FIVE);
    EXPECT_EQ(Env::winamaxOcr().readCardRank(seven), SEVEN);
    EXPECT_EQ(Env::winamaxOcr().readCardRank(eight), EIGHT);
    EXPECT_EQ(Env::winamaxOcr().readCardRank(queen), QUEEN);
    EXPECT_EQ(Env::winamaxOcr().readCardRank(king), KING);
}

TEST(WinamaxOcrTest, readCardRankShouldReturnUndefinedOnUnknowSuitColor) {
    auto unknown = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/suits/unknown.png");

    EXPECT_EQ(Env::winamaxOcr().readCardRank(unknown), UNDEFINED);
}

TEST(WinamaxOcrTest, readCardSuitShouldWork) {
    auto club    = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/suits/club.png");
    auto diamond = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/suits/diamond.png");
    auto heart   = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/suits/heart.png");
    auto spade   = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/suits/spade.png");

    EXPECT_EQ(Env::winamaxOcr().readCardSuit(club), CLUB);
    EXPECT_EQ(Env::winamaxOcr().readCardSuit(diamond), DIAMOND);
    EXPECT_EQ(Env::winamaxOcr().readCardSuit(heart), HEART);
    EXPECT_EQ(Env::winamaxOcr().readCardSuit(spade), SPADE);
}

TEST(WinamaxOcrTest, readCardSuitShouldReturnUnkownOnUnknowSuitColor) {
    auto unknown = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/suits/unknown.png");

    EXPECT_EQ(Env::winamaxOcr().readCardSuit(unknown), UNKNOWN);
}

TEST(WinamaxOcrTest, readCardShouldWork) {
    auto heightOfDiamond = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/cards/8D.png");
    auto nineOfSpade     = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/cards/9S.png");
    auto aceOfSpade      = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/cards/AS.png");
    auto twoOfHeart      = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/cards/2H.png");
    auto kingOfClub      = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/cards/KC.png");

    EXPECT_EQ(Env::winamaxOcr().readCard(heightOfDiamond), card("8D"));
    EXPECT_EQ(Env::winamaxOcr().readCard(nineOfSpade), card("9S"));
    EXPECT_EQ(Env::winamaxOcr().readCard(aceOfSpade), card("AS"));
    EXPECT_EQ(Env::winamaxOcr().readCard(twoOfHeart), card("2H"));
    EXPECT_EQ(Env::winamaxOcr().readCard(kingOfClub), card("KC"));
}

TEST(WinamaxOcrTest, readActionShouldWork) {
    auto raisesTxt = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/raises_text.png");

    EXPECT_STREQ(Env::winamaxOcr().readGameAction(raisesTxt).c_str(), "RAISES TO 3 BB");
}

TEST(WinamaxOcrTest, readPlayerNameShouldWork) {
    auto player1NameImg = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/player_name/name_1.png");
    auto player2NameImg = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/player_name/name_2.png");
    auto player3NameImg = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/player_name/name_3.png");

    EXPECT_STREQ(Env::winamaxOcr().readPlayerName(player1NameImg).c_str(), "_Mister_");
    //    EXPECT_STREQ(Env::winamaxOcr().readPlayerName(player2NameImg).c_str(), "G00dspeed"); // @todo fixme
    EXPECT_STREQ(Env::winamaxOcr().readPlayerName(player3NameImg).c_str(), "sucre_461406");
}

TEST(WinamaxOcrTest, readBetAmountShouldWork) {
    auto zeroDotFiftyBBImg         = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/bets/0_dot_50_BB.png");
    auto oneBBImg                  = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/bets/1_BB.png");
    auto threeHundredAndFifteenImg = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/bets/315.png");

    EXPECT_EQ(Env::winamaxOcr().readPlayerBetInBB(zeroDotFiftyBBImg), 0.5);
    EXPECT_EQ(Env::winamaxOcr().readPlayerBetInBB(oneBBImg), 1);
    EXPECT_EQ(Env::winamaxOcr().readPlayerBet(threeHundredAndFifteenImg), 315);
}

TEST(WinamaxOcrTest, readPotAmountShouldWork) {
    auto oneDotFiftyBBImg       = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/pot/1_dot_50_BB.png");
    auto fourBBImg              = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/pot/4_BB.png");
    auto sixHundredAndThirtyImg = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/pot/630.png");

    EXPECT_EQ(Env::winamaxOcr().readPotInBB(oneDotFiftyBBImg), 1.5);
    EXPECT_EQ(Env::winamaxOcr().readPotInBB(fourBBImg), 4);
    EXPECT_EQ(Env::winamaxOcr().readPot(sixHundredAndThirtyImg), 630);
}

TEST(WinamaxOcrTest, DISABLED_readPrizePoolAmountShouldWork) {
    // @todo Learn tesseract on specific font https://github.com/tesseract-ocr/tesstrain https://www.youtube.com/watch?v=KE4xEzFGSU8
    // @link https://www.myfonts.com/products/fat-corner-b-276039
    // @link https://www.myfonts.com/products/wide-blunt-183803
    // @link https://www.myfonts.com/products/blind-leco-1988-273748
    auto twenty = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/prize_pool/20.png");

    EXPECT_EQ(Env::winamaxOcr().readPrizePool(twenty), 20);
}

TEST(WinamaxOcrTest, readBlindLevelShouldWork) {
    auto two   = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/blind_level/2.png");
    auto three = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/blind_level/3.png");

    EXPECT_EQ(Env::winamaxOcr().readPrizePool(two), 2);
    EXPECT_EQ(Env::winamaxOcr().readPrizePool(three), 3);
}

TEST(WinamaxOcrTest, readBlindAmountShouldWork) {
    auto fifteenToThirty = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/blind_amount/15_30.png");
    auto twentyToForty   = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/blind_amount/20_40.png");

    EXPECT_EQ(Env::winamaxOcr().readSmallBlind(fifteenToThirty), 15);
    EXPECT_EQ(Env::winamaxOcr().readBigBlind(fifteenToThirty), 30);
    EXPECT_EQ(Env::winamaxOcr().readSmallBlind(twentyToForty), 20);
    EXPECT_EQ(Env::winamaxOcr().readBigBlind(twentyToForty), 40);
}

TEST(WinamaxOcrTest, readGameTimeDurationShouldWork) {
    auto oneMinute          = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/game_duration/1m.png");
    auto eighteenSeconds    = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/game_duration/18s.png");
    auto fiftyHeightSeconds = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/game_duration/58s.png");

    EXPECT_EQ(Env::winamaxOcr().readGameDuration(oneMinute), std::chrono::seconds(60));
    EXPECT_EQ(Env::winamaxOcr().readGameDuration(eighteenSeconds), std::chrono::seconds(18));
    EXPECT_EQ(Env::winamaxOcr().readGameDuration(fiftyHeightSeconds), std::chrono::seconds(58));
}

TEST(WinamaxOcrTest, playerHasFoldedShouldWork) {
    auto cardsSkin = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/cards_skins/skin_1.png");
    auto hands     = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/cards_skins/hand.png");

    EXPECT_TRUE(Env::winamaxOcr().hasFolded(hands));
    EXPECT_FALSE(Env::winamaxOcr().hasFolded(cardsSkin));
}

TEST(WinamaxOcrTest, playerButtonDetectionShouldWork) {
    auto button   = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/screen_btn.png");
    auto noButton = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/screen_no_btn.png");

    EXPECT_TRUE(Env::winamaxOcr().hasButton(button));
    EXPECT_FALSE(Env::winamaxOcr().hasButton(noButton));
}

auto main(int argc, char* argv[]) -> int {
    ::testing::InitGoogleTest(&argc, argv);
    // gtest takes ownership of the TestEnvironment ptr - we don't delete it.
    ::testing::AddGlobalTestEnvironment(new Env);
    return RUN_ALL_TESTS();
}
