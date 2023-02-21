#pragma once

#include <ScraperInterface.hpp>

namespace Scraper {
    class WinamaxScraper : public ScraperInterface {
        public:
            WinamaxScraper()                            = default;
            WinamaxScraper(const WinamaxScraper& other) = default;
            WinamaxScraper(WinamaxScraper&& other) noexcept { *this = std::move(other); };

            ~WinamaxScraper() override = default;

            auto operator=(const WinamaxScraper& other) -> WinamaxScraper& = default;
            auto operator=(WinamaxScraper&& other) noexcept -> WinamaxScraper&;

            auto getFirstCardCoordinate() -> cv::Rect override { return {}; }
            auto getSecondCardCoordinate() -> cv::Rect override { return {}; }
            auto getPotCoordinate() -> cv::Rect override { return {}; }
            auto getPrizePoolCoordinate() -> cv::Rect override { return {}; }
            auto getBoardCard1Coordinate() -> cv::Rect override { return {}; }
            auto getBoardCard2Coordinate() -> cv::Rect override { return {}; }
            auto getBoardCard3Coordinate() -> cv::Rect override { return {}; }
            auto getBoardCard4Coordinate() -> cv::Rect override { return {}; }
            auto getBoardCard5Coordinate() -> cv::Rect override { return {}; }
            auto getPlayer1NameCoordinate() -> cv::Rect override { return {}; }
            auto getPlayer2NameCoordinate() -> cv::Rect override { return {}; }
            auto getPlayer3NameCoordinate() -> cv::Rect override { return {}; }
            auto getPlayer1ButtonCoordinate() -> cv::Rect override { return {}; }
            auto getPlayer2ButtonCoordinate() -> cv::Rect override { return {}; }
            auto getPlayer3ButtonCoordinate() -> cv::Rect override { return {}; }
            auto getPlayer1StackCoordinate() -> cv::Rect override { return {}; }
            auto getPlayer2StackCoordinate() -> cv::Rect override { return {}; }
            auto getPlayer3StackCoordinate() -> cv::Rect override { return {}; }
            auto getPlayer1BetCoordinate() -> cv::Rect override { return {}; }
            auto getPlayer2BetCoordinate() -> cv::Rect override { return {}; }
            auto getPlayer3BetCoordinate() -> cv::Rect override { return {}; }
            auto getPlayer2CardsCoordinate() -> cv::Rect override { return {}; }
            auto getPlayer3CardsCoordinate() -> cv::Rect override { return {}; }
    };
}  // namespace Scraper
