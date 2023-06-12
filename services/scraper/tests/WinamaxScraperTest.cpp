#include <gtest/gtest.h>

#include <OcrFactory.hpp>
#include <WinamaxScraper.hpp>

using OCR::Factory::OcrFactory;
using Scraper::WinamaxScraper;

class WinamaxScraperTest : public ::testing::Test {};

TEST(WinamaxScraperTest, displayScreenshot) {
    WinamaxScraper scraper;

    for (const auto& [id, window] : scraper.getActiveWindows())
    {
        fmt::print("({}) - {}\n", id, window.title);
        //        cv::imshow(window.title, scraper.getScreenshot(id));
    }

    //    cv::waitKey(-1);
}

TEST(WinamaxScraperTest, displayAllElements) {
    WinamaxScraper scraper;

    cv::Mat screenshot_1 = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/screen_1.png");
    cv::Mat screenshot_2 = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/screen_2.png");
    cv::Mat screenshot_3 = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/screen_3.png");
    cv::Mat screenshot_4 = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/screen_4.png");
    cv::Mat screenshot_5 = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/screen_5.png");
    cv::Mat screenshot_6 = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/screen_6.png");

    cv::imshow("screen_1", scraper.getWindowElementsView(screenshot_1));
    cv::imshow("screen_2", scraper.getWindowElementsView(screenshot_2));
    cv::imshow("screen_3", scraper.getWindowElementsView(screenshot_3));
    cv::imshow("screen_4", scraper.getWindowElementsView(screenshot_4));
    cv::imshow("screen_5", scraper.getWindowElementsView(screenshot_5));
    cv::imshow("screen_6", scraper.getWindowElementsView(screenshot_6));
    //    cv::imshow("screen_6_player_1", scraper.getPlayer1NameImg(screenshot_6));
    //    cv::imshow("screen_6_player_2", scraper.getPlayer2NameImg(screenshot_6));

    cv::waitKey(-1);
}

TEST(WinamaxScraperTest, shouldScrapPlayersName) {
    WinamaxScraper scraper;
    auto           ocr = OcrFactory::create("Winamax");

    cv::Mat screenshot     = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/screen_3.png");
    cv::Mat player1NameImg = scraper.getPlayer1NameImg(screenshot);
    cv::Mat player2NameImg = scraper.getPlayer2NameImg(screenshot);
    cv::Mat player3NameImg = scraper.getPlayer3NameImg(screenshot);

    cv::imshow("player1NameImg", player1NameImg);
    cv::imshow("player2NameImg", player2NameImg);
    cv::imshow("player3NameImg", player3NameImg);

    EXPECT_STREQ(ocr->readWord(player1NameImg).c_str(), "_Mister_");
    EXPECT_STREQ(ocr->readWord(player2NameImg).c_str(), "G00dspeed");
    EXPECT_STREQ(ocr->readWord(player3NameImg).c_str(), "sucre_461406");

    cv::imshow("player1NameImg processed", player1NameImg);
    cv::imshow("player2NameImg processed", player2NameImg);
    cv::imshow("player3NameImg processed", player3NameImg);
    cv::waitKey(-1);
}
