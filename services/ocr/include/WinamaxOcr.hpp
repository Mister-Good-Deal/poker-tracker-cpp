#pragma once

#include <OcrInterface.hpp>

namespace OCR {
    class WinamaxOcr final : public OcrInterface {
        public:
            WinamaxOcr()                        = default;
            WinamaxOcr(const WinamaxOcr& other) = default;
            WinamaxOcr(WinamaxOcr&& other) noexcept { *this = std::move(other); };

            ~WinamaxOcr() final = default;

            auto operator=(const WinamaxOcr& other) -> WinamaxOcr& = default;
            auto operator=(WinamaxOcr&& other) noexcept -> WinamaxOcr&;

            auto setCardsSkin(const cv::Mat& cardsSkin) -> void { _cardsSkin = cardsSkin; }

            [[nodiscard]] auto getButtonMask() const -> cv::Mat override;
            [[nodiscard]] auto getButtonImg() const -> cv::Mat override {
                return cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/dealer_btn.png");
            }

            // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
            [[nodiscard]] auto getRankCardArea() const -> cv::Rect override { return {0, 0, 20, 23}; }
            [[nodiscard]] auto getSuitCardArea() const -> cv::Rect override { return {2, 25, 14, 16}; }
            // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
            auto readCardRank(cv::Mat& rankImage) const -> Card::Rank override;
            auto readCardSuit(cv::Mat& suitImage) const -> Card::Suit override;
            auto readPlayerName(cv::Mat& playerNameImage) const -> std::string override;
            auto readGameAction(cv::Mat& playerNameImage) const -> std::string;
            auto hasFolded(cv::Mat& handImage) const -> bool;

        private:
            cv::Mat _cardsSkin;

            [[nodiscard]] auto _cvColorToString(const cv::Vec3b& color) const -> std::string;

            auto _processImageBeforeWhiteTextOcr(cv::Mat& image) const -> void;
            auto _processImageBeforeYellowTextOcr(cv::Mat& image) const -> void;
            auto _colorRangeThreshold(cv::Mat& image, const cv::Scalar& colorLower, const cv::Scalar& colorUpper) const -> void;
    };
}  // namespace OCR