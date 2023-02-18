#pragma once

#include "OcrInterface.hpp"

namespace OCR {
    class WinamaxOcr final : public OcrInterface {
        public:
            static constexpr double SIMILARITY_THRESHOLD = 0.99;

            WinamaxOcr()                        = default;
            WinamaxOcr(const WinamaxOcr& other) = default;
            WinamaxOcr(WinamaxOcr&& other) noexcept { *this = std::move(other); };

            ~WinamaxOcr() final = default;

            auto operator=(const WinamaxOcr& other) -> WinamaxOcr& = default;
            auto operator=(WinamaxOcr&& other) noexcept -> WinamaxOcr&;

            auto setCardsSkin(const cv::Mat& cardsSkin) -> void { _cardsSkin = cardsSkin; }

            auto readCardRank(cv::Mat& rankImage) const -> Card::Rank override;
            auto readCardSuit(cv::Mat& suitImage) const -> Card::Suit override;
            auto hasFolded(cv::Mat& cardsSkinImage) const -> bool;

        private:
            cv::Mat _cardsSkin;

            [[nodiscard]] auto _cvColorToString(const cv::Vec3b& color) const -> std::string;
    };
}  // namespace OCR