#pragma once

#include <ScraperInterface.hpp>

namespace Scraper {
    class WinamaxScraper : public ScraperInterface {
        public:
            // Size in pixels
            static const int32_t PADDING_TOP                  = 20;
            static const int32_t BOARD_CARDS_WIDTH_SEPARATION = 54;
            static const int32_t BOARD_CARDS_Y_COORDINATE     = 193 + PADDING_TOP;
            static const int32_t BET_WIDTH                    = 90;
            static const int32_t BET_HEIGHT                   = 18;
            static const int32_t BUTTON_WIDTH                 = 25;
            static const int32_t BUTTON_HEIGHT                = 25;
            static const int32_t CARD_WIDTH                   = 51;
            static const int32_t CARD_HEIGHT                  = 77;
            static const int32_t HAND_WIDTH                   = 72;
            static const int32_t HAND_HEIGHT                  = 41;
            static const int32_t NAME_WIDTH                   = 130;
            static const int32_t NAME_HEIGHT                  = 18;
            static const int32_t STACK_WIDTH                  = 100;
            static const int32_t STACK_HEIGHT                 = 19;

            WinamaxScraper()                            = default;
            WinamaxScraper(const WinamaxScraper& other) = default;
            WinamaxScraper(WinamaxScraper&& other) noexcept { *this = std::move(other); };

            ~WinamaxScraper() override = default;

            auto operator=(const WinamaxScraper& other) -> WinamaxScraper& = default;
            auto operator=(WinamaxScraper&& other) noexcept -> WinamaxScraper&;

            // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
            auto getFirstCardCoordinate() const -> const cv::Rect override {
                return {291, 350 + PADDING_TOP, CARD_WIDTH, HAND_HEIGHT};
            }
            auto getSecondCardCoordinate() const -> const cv::Rect override {
                return {312, 350 + PADDING_TOP, CARD_WIDTH, HAND_HEIGHT};
            }
            auto getPotCoordinate() const -> const cv::Rect override { return {269, 285 + PADDING_TOP, 132, 23}; }
            auto getPrizePoolCoordinate() const -> const cv::Rect override { return {229, 220 + PADDING_TOP, 211, 60}; }
            auto getBoardCard1Coordinate() const -> const cv::Rect override;
            auto getBoardCard2Coordinate() const -> const cv::Rect override;
            auto getBoardCard3Coordinate() const -> const cv::Rect override;
            auto getBoardCard4Coordinate() const -> const cv::Rect override;
            auto getBoardCard5Coordinate() const -> const cv::Rect override;
            auto getPlayer1NameCoordinate() const -> const cv::Rect override {
                return {263, 392 + PADDING_TOP, NAME_WIDTH, NAME_HEIGHT};
            }
            auto getPlayer2NameCoordinate() const -> const cv::Rect override {
                return {40, 138 + PADDING_TOP, NAME_WIDTH, NAME_HEIGHT};
            }
            auto getPlayer3NameCoordinate() const -> const cv::Rect override {
                return {490, 138 + PADDING_TOP, NAME_WIDTH, NAME_HEIGHT};
            }
            auto getPlayer1ButtonCoordinate() const -> const cv::Rect override {
                return {255, 333 + PADDING_TOP, BUTTON_WIDTH, BUTTON_HEIGHT};
            }
            auto getPlayer2ButtonCoordinate() const -> const cv::Rect override {
                return {170, 134 + PADDING_TOP, BUTTON_WIDTH, BUTTON_HEIGHT};
            }
            auto getPlayer3ButtonCoordinate() const -> const cv::Rect override {
                return {528, 206 + PADDING_TOP, BUTTON_WIDTH, BUTTON_HEIGHT};
            }
            auto getPlayer1StackCoordinate() const -> const cv::Rect override {
                return {278, 409 + PADDING_TOP, STACK_WIDTH, STACK_HEIGHT};
            }
            auto getPlayer2StackCoordinate() const -> const cv::Rect override {
                return {54, 153 + PADDING_TOP, STACK_WIDTH, STACK_HEIGHT};
            }
            auto getPlayer3StackCoordinate() const -> const cv::Rect override {
                return {508, 153 + PADDING_TOP, STACK_WIDTH, STACK_HEIGHT};
            }
            auto getPlayer1BetCoordinate() const -> const cv::Rect override { return {287, 332 + PADDING_TOP, BET_WIDTH, BET_HEIGHT}; }
            auto getPlayer2BetCoordinate() const -> const cv::Rect override { return {135, 184 + PADDING_TOP, BET_WIDTH, BET_HEIGHT}; }
            auto getPlayer3BetCoordinate() const -> const cv::Rect override { return {435, 184 + PADDING_TOP, BET_WIDTH, BET_HEIGHT}; }
            auto getPlayer2HandCoordinate() const -> const cv::Rect override {
                return {67, 97 + PADDING_TOP, HAND_WIDTH, HAND_HEIGHT};
            }
            auto getPlayer3HandCoordinate() const -> const cv::Rect override {
                return {515, 97 + PADDING_TOP, HAND_WIDTH, HAND_HEIGHT};
            }
            auto getBlindLevelCoordinate() const -> const cv::Rect override { return {485, 28 + PADDING_TOP, 12, 18}; }
            auto getBlindAmountCoordinate() const -> const cv::Rect override { return {502, 28 + PADDING_TOP, 45, 18}; }
            auto getGameTimeCoordinate() const -> const cv::Rect override { return {545, 28 + PADDING_TOP, 45, 18}; }
            // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
    };
}  // namespace Scraper
