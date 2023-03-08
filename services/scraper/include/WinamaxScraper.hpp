#pragma once

#include <ScraperInterface.hpp>

namespace Scraper {
    class WinamaxScraper : public ScraperInterface {
        public:
            // Size in pixels
            static const int32_t BOARD_CARDS_WIDTH_SEPARATION = 56;
            static const int32_t BOARD_CARDS_Y_COORDINATE     = 196;
            static const int32_t BET_WIDTH                    = 110;
            static const int32_t BET_HEIGHT                   = 18;
            static const int32_t BUTTON_WIDTH                 = 23;
            static const int32_t BUTTON_HEIGHT                = 23;
            static const int32_t CARD_WIDTH                   = 51;
            static const int32_t CARD_HEIGHT                  = 77;
            static const int32_t HAND_WIDTH                   = 72;
            static const int32_t HAND_HEIGHT                  = 41;
            static const int32_t NAME_WIDTH                   = 130;
            static const int32_t NAME_HEIGHT                  = 16;
            static const int32_t STACK_WIDTH                  = 100;
            static const int32_t STACK_HEIGHT                 = 17;

            WinamaxScraper()                            = default;
            WinamaxScraper(const WinamaxScraper& other) = default;
            WinamaxScraper(WinamaxScraper&& other) noexcept { *this = std::move(other); };

            ~WinamaxScraper() override = default;

            auto operator=(const WinamaxScraper& other) -> WinamaxScraper& = default;
            auto operator=(WinamaxScraper&& other) noexcept -> WinamaxScraper&;

            // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
            auto getFirstCardCoordinate() -> cv::Rect override { return {300, 356, CARD_WIDTH, CARD_HEIGHT}; }
            auto getSecondCardCoordinate() -> cv::Rect override { return {321, 356, CARD_WIDTH, CARD_HEIGHT}; }
            auto getPotCoordinate() -> cv::Rect override { return {269, 289, 132, 20}; }
            auto getPrizePoolCoordinate() -> cv::Rect override { return {229, 220, 211, 60}; }
            auto getBoardCard1Coordinate() -> cv::Rect override;
            auto getBoardCard2Coordinate() -> cv::Rect override;
            auto getBoardCard3Coordinate() -> cv::Rect override;
            auto getBoardCard4Coordinate() -> cv::Rect override;
            auto getBoardCard5Coordinate() -> cv::Rect override;
            auto getPlayer1NameCoordinate() -> cv::Rect override { return {270, 397, NAME_WIDTH, NAME_HEIGHT}; }
            auto getPlayer2NameCoordinate() -> cv::Rect override { return {40, 136, NAME_WIDTH, NAME_HEIGHT}; }
            auto getPlayer3NameCoordinate() -> cv::Rect override { return {501, 136, NAME_WIDTH, NAME_HEIGHT}; }
            auto getPlayer1ButtonCoordinate() -> cv::Rect override { return {}; }
            auto getPlayer2ButtonCoordinate() -> cv::Rect override { return {172, 133, BUTTON_WIDTH, BUTTON_HEIGHT}; }
            auto getPlayer3ButtonCoordinate() -> cv::Rect override { return {528, 206, BUTTON_WIDTH, BUTTON_HEIGHT}; }
            auto getPlayer1StackCoordinate() -> cv::Rect override { return {285, 414, STACK_WIDTH, STACK_HEIGHT}; }
            auto getPlayer2StackCoordinate() -> cv::Rect override { return {54, 153, STACK_WIDTH, STACK_HEIGHT}; }
            auto getPlayer3StackCoordinate() -> cv::Rect override { return {516, 153, STACK_WIDTH, STACK_HEIGHT}; }
            auto getPlayer1BetCoordinate() -> cv::Rect override { return {300, 337, BET_WIDTH, BET_HEIGHT}; }
            auto getPlayer2BetCoordinate() -> cv::Rect override { return {51, 156, BET_WIDTH, BET_HEIGHT}; }
            auto getPlayer3BetCoordinate() -> cv::Rect override { return {511, 156, BET_WIDTH, BET_HEIGHT}; }
            auto getPlayer2HandCoordinate() -> cv::Rect override { return {69, 95, HAND_WIDTH, HAND_HEIGHT}; }
            auto getPlayer3HandCoordinate() -> cv::Rect override { return {531, 95, HAND_WIDTH, HAND_HEIGHT}; }
            // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
    };
}  // namespace Scraper
