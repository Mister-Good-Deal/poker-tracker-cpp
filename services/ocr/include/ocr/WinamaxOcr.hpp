#pragma once

#include <ocr/OcrInterface.hpp>

namespace OCR {
    using uniqueMat_t = std::unique_ptr<cv::Mat>;

    class WinamaxOcr final : public OcrInterface {
        public:
            static constexpr std::string DEFAULT_CARD_SKIN              = "skin_1.png";
            static constexpr std::string DEFAULT_BUTTON_IMG             = "dealer_btn.png";
            static constexpr int32_t     DEFAULT_OCR_IMG_WIDTH          = 1000;
            static constexpr int32_t     DEFAULT_MIN_CLUSTER_SIZE       = 30;
            static constexpr int32_t     DEFAULT_DILATATION_KERNEL_SIZE = 5;
            static constexpr int32_t     CARD_WIDTH                     = 51;

            WinamaxOcr();
            WinamaxOcr(const WinamaxOcr& other) = default;
            WinamaxOcr(WinamaxOcr&& other) noexcept : OcrInterface(std::move(other)) { *this = std::move(other); };
            explicit WinamaxOcr(cv::Mat cardsSkin) : _cardsSkin(std::move(cardsSkin)), OcrInterface(CARD_WIDTH){};

            ~WinamaxOcr() final = default;

            auto operator=(const WinamaxOcr& other) -> WinamaxOcr& = default;
            auto operator=(WinamaxOcr&& other) noexcept -> WinamaxOcr&;

            auto setCardsSkin(const cv::Mat& cardsSkin) -> void { _cardsSkin = cardsSkin; }

            [[nodiscard]] auto getButtonImg() const -> cv::Mat override;
            [[nodiscard]] auto getButtonMask() const -> cv::Mat override;
            [[nodiscard]] auto getRankCardArea() const -> cv::Rect override;
            [[nodiscard]] auto getSuitCardArea() const -> cv::Rect override;
            [[nodiscard]] auto hasButton(const cv::Mat& buttonImage) const -> bool override;
            [[nodiscard]] auto hasFolded(const cv::Mat& handImage) const -> bool override;
            [[nodiscard]] auto readAverageStack(const cv::Mat& averageStackImage) const -> int32_t override;
            [[nodiscard]] auto readBigBlind(const cv::Mat& blindRangeImage) const -> int32_t override;
            [[nodiscard]] auto readBlindLevel(const cv::Mat& blindLevelImage) const -> int32_t override;
            [[nodiscard]] auto readBlindLevelDuration(const cv::Mat& durationImage) const -> seconds override;
            [[nodiscard]] auto readBlindRange(const cv::Mat& blindRangeImage) const -> Blinds override;
            [[nodiscard]] auto readCardRank(const cv::Mat& rankImage) const -> Card::Rank override;
            [[nodiscard]] auto readCardSuit(const cv::Mat& suitImage) const -> Card::Suit override;
            [[nodiscard]] auto readGameAction(const cv::Mat& actionImage) const -> ActionType override;
            [[nodiscard]] auto readPlayerBet(const cv::Mat& playerBetImage) const -> int32_t override;
            [[nodiscard]] auto readPlayerBetInBB(const cv::Mat& playerBetInBBImage) const -> double override;
            [[nodiscard]] auto readPlayerName(const cv::Mat& playerNameImage) const -> std::string override;
            [[nodiscard]] auto readPot(const cv::Mat& potImage) const -> int32_t override;
            [[nodiscard]] auto readPotInBB(const cv::Mat& potInBBImage) const -> double override;
            [[nodiscard]] auto readPrizePool(const cv::Mat& prizePoolImage) const -> int32_t override;
            [[nodiscard]] auto readSmallBlind(const cv::Mat& blindRangeImage) const -> int32_t override;

        private:
            cv::Mat _cardsSkin;
            cv::Mat _buttonImg;

            auto _extractWhiteTextDarkBackground(cv::Mat& img) const -> cv::Mat&;
            auto _extractWhiteTextLightBackground(cv::Mat& img) const -> cv::Mat&;
            auto _extractYellowText(cv::Mat& img) const -> cv::Mat&;
            auto _colorRangeThreshold(cv::Mat& img, const cv::Scalar& colorLower, const cv::Scalar& colorUpper) const -> cv::Mat&;
            auto _trimSalt(cv::Mat& img, int32_t minClusterSize = DEFAULT_MIN_CLUSTER_SIZE) const -> cv::Mat&;
            auto _applyDilation(cv::Mat& img, int32_t kernelSize = DEFAULT_DILATATION_KERNEL_SIZE) const -> cv::Mat&;

            [[nodiscard]] auto _resizedImage(const cv::Mat& src, int32_t desiredWidth = DEFAULT_OCR_IMG_WIDTH) const -> uniqueMat_t;
    };
}  // namespace OCR