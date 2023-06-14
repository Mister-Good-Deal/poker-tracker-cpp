#pragma once

#include <OcrInterface.hpp>

namespace OCR {
    class WinamaxOcr final : public OcrInterface {
        public:
            static constexpr std::string DEFAULT_CARD_SKIN  = "skin_1.png";
            static constexpr std::string DEFAULT_BUTTON_IMG = "dealer_btn.png";

            WinamaxOcr();
            WinamaxOcr(const WinamaxOcr& other) = default;
            WinamaxOcr(WinamaxOcr&& other) noexcept { *this = std::move(other); };
            explicit WinamaxOcr(cv::Mat cardsSkin) : _cardsSkin(std::move(cardsSkin)){};

            ~WinamaxOcr() final = default;

            auto operator=(const WinamaxOcr& other) -> WinamaxOcr& = default;
            auto operator=(WinamaxOcr&& other) noexcept -> WinamaxOcr&;

            auto setCardsSkin(const cv::Mat& cardsSkin) -> void { _cardsSkin = cardsSkin; }

            [[nodiscard]] auto getButtonMask() const -> cv::Mat override;
            [[nodiscard]] auto getButtonImg() const -> cv::Mat override;
            [[nodiscard]] auto getRankCardArea() const -> cv::Rect override;
            [[nodiscard]] auto getSuitCardArea() const -> cv::Rect override;
            [[nodiscard]] auto readCardRank(const cv::Mat& rankImage) const -> Card::Rank override;
            [[nodiscard]] auto readCardSuit(const cv::Mat& suitImage) const -> Card::Suit override;
            [[nodiscard]] auto readPlayerName(const cv::Mat& playerNameImage) const -> std::string override;
            [[nodiscard]] auto readGameAction(const cv::Mat& actionImage) const -> std::string;
            [[nodiscard]] auto hasFolded(const cv::Mat& handImage) const -> bool;
            [[nodiscard]] auto hasButton(const cv::Mat& buttonImage) const -> bool;

        private:
            cv::Mat _cardsSkin;
            cv::Mat _buttonImg;

            [[nodiscard]] auto _cvColorToString(const cv::Vec3b& color) const -> std::string;
            [[nodiscard]] auto _extractWhiteText(const cv::Mat& image) const -> cv::Mat;
            [[nodiscard]] auto _extractYellowText(const cv::Mat& image) const -> cv::Mat;
            [[nodiscard]] auto _colorRangeThreshold(const cv::Mat& image, const cv::Scalar& colorLower,
                                                    const cv::Scalar& colorUpper) const -> cv::Mat;
    };
}  // namespace OCR