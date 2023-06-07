#include <gtest/gtest.h>

#include <WinamaxScraper.hpp>

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

    cv::imshow("screen_1", scraper.getWindowElementsView(screenshot_1));
    cv::imshow("screen_2", scraper.getWindowElementsView(screenshot_2));
    cv::imshow("screen_3", scraper.getWindowElementsView(screenshot_3));
    cv::imshow("screen_4", scraper.getWindowElementsView(screenshot_4));
    cv::imshow("screen_5", scraper.getWindowElementsView(screenshot_5));

    cv::waitKey(-1);
}
