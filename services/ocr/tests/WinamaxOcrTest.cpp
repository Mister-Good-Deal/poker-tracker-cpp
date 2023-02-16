#include <gtest/gtest.h>

#include <opencv4/opencv2/imgcodecs.hpp>

#include "CardFactory.hpp"
#include "WinamaxOcr.hpp"

using GameHandler::Factory::card;
using OCR::WinamaxOcr;

using enum GameHandler::Card::Suit;
using enum GameHandler::Card::Rank;

#define EXPECT_THROW_WITH_MESSAGE(code, exception_type, expected_message) \
    do                                                                    \
    {                                                                     \
        EXPECT_THROW(                                                     \
            try { code; } catch (const exception_type& e) {               \
                EXPECT_STREQ(e.what(), expected_message);                 \
                throw;                                                    \
            },                                                            \
            exception_type);                                              \
    } while (0)

class Env : public ::testing::Environment {
    public:
        static auto winamaxOcr() -> const WinamaxOcr {
            static const WinamaxOcr ocr;

            return ocr;
        };
};

class WinamaxOcrTest : public ::testing::Test {};

TEST(WinamaxOcrTest, readCardRankShouldWork) {
    auto two   = cv::imread(std::string(RANK_IMAGES_DIR) + "/2.png");
    auto four  = cv::imread(std::string(RANK_IMAGES_DIR) + "/4.png");
    auto five  = cv::imread(std::string(RANK_IMAGES_DIR) + "/5.png");
    auto seven = cv::imread(std::string(RANK_IMAGES_DIR) + "/7.png");
    auto eight = cv::imread(std::string(RANK_IMAGES_DIR) + "/8.png");
    auto queen = cv::imread(std::string(RANK_IMAGES_DIR) + "/Q.png");
    auto king  = cv::imread(std::string(RANK_IMAGES_DIR) + "/K.png");

    EXPECT_EQ(Env::winamaxOcr().readCardRank(two), TWO);
    EXPECT_EQ(Env::winamaxOcr().readCardRank(four), FOUR);
    EXPECT_EQ(Env::winamaxOcr().readCardRank(five), FIVE);
    EXPECT_EQ(Env::winamaxOcr().readCardRank(seven), SEVEN);
    EXPECT_EQ(Env::winamaxOcr().readCardRank(eight), EIGHT);
    EXPECT_EQ(Env::winamaxOcr().readCardRank(queen), QUEEN);
    EXPECT_EQ(Env::winamaxOcr().readCardRank(king), KING);
}

TEST(WinamaxOcrTest, readCardRankShouldReturnUndefinedOnUnknowSuitColor) {
    auto unknown = cv::imread(std::string(SUIT_IMAGES_DIR) + "/unknown.png");

    EXPECT_EQ(Env::winamaxOcr().readCardRank(unknown), UNDEFINED);
}

TEST(WinamaxOcrTest, readCardSuitShouldWork) {
    auto club    = cv::imread(std::string(SUIT_IMAGES_DIR) + "/club.png");
    auto diamond = cv::imread(std::string(SUIT_IMAGES_DIR) + "/diamond.png");
    auto heart   = cv::imread(std::string(SUIT_IMAGES_DIR) + "/heart.png");
    auto spade   = cv::imread(std::string(SUIT_IMAGES_DIR) + "/spade.png");

    EXPECT_EQ(Env::winamaxOcr().readCardSuit(club), CLUB);
    EXPECT_EQ(Env::winamaxOcr().readCardSuit(diamond), DIAMOND);
    EXPECT_EQ(Env::winamaxOcr().readCardSuit(heart), HEART);
    EXPECT_EQ(Env::winamaxOcr().readCardSuit(spade), SPADE);
}

TEST(WinamaxOcrTest, readCardSuitShouldReturnUnkownOnUnknowSuitColor) {
    auto unknown = cv::imread(std::string(SUIT_IMAGES_DIR) + "/unknown.png");

    EXPECT_EQ(Env::winamaxOcr().readCardSuit(unknown), UNKNOWN);
}

TEST(WinamaxOcrTest, readCardShouldWork) {
    auto club  = cv::imread(std::string(SUIT_IMAGES_DIR) + "/club.png");
    auto seven = cv::imread(std::string(RANK_IMAGES_DIR) + "/7.png");

    EXPECT_EQ(Env::winamaxOcr().readCard(seven, club), card("7C"));
}

auto main(int argc, char* argv[]) -> int {
    ::testing::InitGoogleTest(&argc, argv);
    // gtest takes ownership of the TestEnvironment ptr - we don't delete it.
    ::testing::AddGlobalTestEnvironment(new Env);
    return RUN_ALL_TESTS();
}
