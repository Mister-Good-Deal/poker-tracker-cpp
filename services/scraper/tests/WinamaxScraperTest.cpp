#include <gtest/gtest.h>

#include "WinamaxScraper.hpp"

using Scraper::WinamaxScraper;

class WinamaxScraperTest : public ::testing::Test {};

TEST(WinamaxScraperTest, displayScreenshot) {
    WinamaxScraper scraper;

    cv::imshow("screenshot", scraper.getScreenshot("rom1@msi-P100: ~/Projects/poker-bot"));
    cv::waitKey(-1);
}
