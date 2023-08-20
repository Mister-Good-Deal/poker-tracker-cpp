#include <gtest/gtest.h>

#include <opencv4/opencv2/imgcodecs.hpp>

#include <game_handler/CardFactory.hpp>
#include <ocr/WinamaxOcr.hpp>
#include <utilities/GtestMacros.hpp>

using GameHandler::Factory::card;
using OCR::UnknownCardRankException;
using OCR::UnknownCardSuitException;
using OCR::WinamaxOcr;

using enum GameHandler::Card::Suit;
using enum GameHandler::Card::Rank;
using enum GameHandler::RoundAction::ActionType;

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

TEST(WinamaxOcrTest, readCardRankShouldThrowErrorOnUnknowRankColor) {
    auto unknown = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/suits/unknown.png");

    EXPECT_THROW(auto rank = Env::winamaxOcr().readCardRank(unknown), UnknownCardRankException);
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

// @todo Suit read is now too permissive
TEST(WinamaxOcrTest, DISABLED_readCardSuitShouldThrowErrorOnUnknowSuitColor) {
    auto unknown = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/suits/unknown.png");

    EXPECT_THROW(auto suit = Env::winamaxOcr().readCardSuit(unknown), UnknownCardSuitException);
}

TEST(WinamaxOcrTest, readCardShouldWork) {
    auto heightOfDiamond = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/cards/8D.png");
    auto nineOfSpade     = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/cards/9S.png");
    auto aceOfSpade      = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/cards/AS.png");
    auto twoOfHeart      = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/cards/2H.png");
    auto kingOfClub      = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/cards/KC.png");

    EXPECT_EQ(Env::winamaxOcr().readPlayerCard(heightOfDiamond), card("8D"));
    EXPECT_EQ(Env::winamaxOcr().readPlayerCard(nineOfSpade), card("9S"));
    EXPECT_EQ(Env::winamaxOcr().readPlayerCard(aceOfSpade), card("AS"));
    EXPECT_EQ(Env::winamaxOcr().readPlayerCard(twoOfHeart), card("2H"));
    EXPECT_EQ(Env::winamaxOcr().readPlayerCard(kingOfClub), card("KC"));
}

TEST(WinamaxOcrTest, readActionShouldWork) {
    auto raisesTxt     = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/actions/raises_to_3_bb.png");
    auto bigBlindTxt   = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/actions/big_blind.png");
    auto smallBlindTxt = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/actions/small_blind.png");
    auto foldTxt       = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/actions/fold.png");

    EXPECT_EQ(Env::winamaxOcr().readGameAction(raisesTxt), RAISE);
    EXPECT_EQ(Env::winamaxOcr().readGameAction(smallBlindTxt), PAY_SMALL_BLIND);
    EXPECT_EQ(Env::winamaxOcr().readGameAction(bigBlindTxt), PAY_BIG_BLIND);
    EXPECT_EQ(Env::winamaxOcr().readGameAction(foldTxt), FOLD);
}

TEST(WinamaxOcrTest, readPlayerNameShouldWork) {
    auto player1NameImg = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/player_name/_Mister_.png");
    auto player2NameImg = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/player_name/G00dspeed.png");
    auto player3NameImg = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/player_name/sucre_461406.png");

    EXPECT_STREQ(Env::winamaxOcr().readPlayerName(player1NameImg).c_str(), "_Mister_");
    // EXPECT_STREQ(Env::winamaxOcr().readPlayerName(player2NameImg).c_str(), "G00dspeed"); @fixme OO instead of 00
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

// @todo Learn tesseract on specific font https://github.com/tesseract-ocr/tesstrain https://www.youtube.com/watch?v=KE4xEzFGSU8
// @link https://www.myfonts.com/products/fat-corner-b-276039
// @link https://www.myfonts.com/products/wide-blunt-183803
// @link https://www.myfonts.com/products/blind-leco-1988-273748
// @issue https://gitlab.laneuville.me/poker-bot/backend/-/issues/31
TEST(WinamaxOcrTest, DISABLED_readPrizePoolAmountShouldWork) {
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

// @todo Learn tesseract on specific font https://github.com/tesseract-ocr/tesstrain https://www.youtube.com/watch?v=KE4xEzFGSU8
// @issue https://gitlab.laneuville.me/poker-bot/backend/-/issues/31
TEST(WinamaxOcrTest, DISABLED_readBlindLevelDurationShouldWork) {
    auto oneMinute          = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/game_duration/1m.png");
    auto eighteenSeconds    = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/game_duration/18s.png");
    auto fiftyHeightSeconds = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/game_duration/58s.png");

    EXPECT_EQ(Env::winamaxOcr().readBlindLevelDuration(oneMinute), std::chrono::seconds(60));
    EXPECT_EQ(Env::winamaxOcr().readBlindLevelDuration(eighteenSeconds), std::chrono::seconds(18));
    EXPECT_EQ(Env::winamaxOcr().readBlindLevelDuration(fiftyHeightSeconds), std::chrono::seconds(58));
}

TEST(WinamaxOcrTest, readAverageStackShouldWork) {
    auto fourHundredAndFifty = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/average_stack/450.png");

    EXPECT_EQ(Env::winamaxOcr().readAverageStack(fourHundredAndFifty), 450);
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

// @todo test readHand

auto main(int argc, char* argv[]) -> int {
    ::testing::InitGoogleTest(&argc, argv);
    // gtest takes ownership of the TestEnvironment ptr - we don't delete it.
    ::testing::AddGlobalTestEnvironment(new Env);
    return RUN_ALL_TESTS();
}
