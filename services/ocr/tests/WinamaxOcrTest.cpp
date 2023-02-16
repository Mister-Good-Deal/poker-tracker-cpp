#include <gtest/gtest.h>

#include <opencv4/opencv2/imgcodecs.hpp>

#include "WinamaxOcr.hpp"

using OCR::UndefinedCardSuit;
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

    EXPECT_TRUE(Env::winamaxOcr().readCardRank(two) == TWO);
    EXPECT_TRUE(Env::winamaxOcr().readCardRank(four) == FOUR);
    EXPECT_TRUE(Env::winamaxOcr().readCardRank(five) == FIVE);
    EXPECT_TRUE(Env::winamaxOcr().readCardRank(seven) == SEVEN);
    EXPECT_TRUE(Env::winamaxOcr().readCardRank(eight) == EIGHT);
    EXPECT_TRUE(Env::winamaxOcr().readCardRank(queen) == QUEEN);
    EXPECT_TRUE(Env::winamaxOcr().readCardRank(king) == KING);
}

TEST(WinamaxOcrTest, readCardSuitShouldWork) {
    auto club    = cv::imread(std::string(SUIT_IMAGES_DIR) + "/club.png");
    auto diamond = cv::imread(std::string(SUIT_IMAGES_DIR) + "/diamond.png");
    auto heart   = cv::imread(std::string(SUIT_IMAGES_DIR) + "/heart.png");
    auto spade   = cv::imread(std::string(SUIT_IMAGES_DIR) + "/spade.png");

    EXPECT_TRUE(Env::winamaxOcr().readCardSuit(club) == CLUB);
    EXPECT_TRUE(Env::winamaxOcr().readCardSuit(diamond) == DIAMOND);
    EXPECT_TRUE(Env::winamaxOcr().readCardSuit(heart) == HEART);
    EXPECT_TRUE(Env::winamaxOcr().readCardSuit(spade) == SPADE);
}

TEST(WinamaxOcrTest, readCardSuitShouldThrowErrorOnUnknowSuitColor) {
    auto unknown = cv::imread(std::string(SUIT_IMAGES_DIR) + "/unknown.png");

    EXPECT_THROW_WITH_MESSAGE(Env::winamaxOcr().readCardSuit(unknown), UndefinedCardSuit,
                              "The middle pixel color BGR(211, 57, 207) does not match a suit");
}

auto main(int argc, char* argv[]) -> int {
    ::testing::InitGoogleTest(&argc, argv);
    // gtest takes ownership of the TestEnvironment ptr - we don't delete it.
    ::testing::AddGlobalTestEnvironment(new Env);
    return RUN_ALL_TESTS();
}
