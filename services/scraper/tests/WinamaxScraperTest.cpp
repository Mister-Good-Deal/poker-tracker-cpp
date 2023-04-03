#include <gtest/gtest.h>

#include "WinamaxScraper.hpp"

using Scraper::WinamaxScraper;

class WinamaxScraperTest : public ::testing::Test {};

TEST(WinamaxScraperTest, displayScreenshot) {
    WinamaxScraper scraper;

    for (const auto& [id, window] : scraper.getActiveWindows())
    { fmt::print("({}) - {}\n", id, window.title); }

    //    cv::imshow("screenshot", scraper.getScreenshot(id));
    //    cv::waitKey(-1);
}
