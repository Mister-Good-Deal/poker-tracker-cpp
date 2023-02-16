#pragma once

#include "OcrInterface.hpp"

namespace OCR {
    class UndefinedCardSuit : public std::runtime_error {
        public:
            explicit UndefinedCardSuit(const std::string& arg) : runtime_error(arg){};
    };

    class WinamaxOcr final : public OcrInterface {
        public:
            WinamaxOcr()                        = default;
            WinamaxOcr(const WinamaxOcr& other) = default;
            WinamaxOcr(WinamaxOcr&& other) noexcept { *this = std::move(other); };

            ~WinamaxOcr() final = default;

            auto operator=(const WinamaxOcr& other) -> WinamaxOcr& = default;
            auto operator=(WinamaxOcr&& other) noexcept -> WinamaxOcr&;

            auto readCardRank(cv::Mat& rankImage) const -> Card::Rank override;
            auto readCardSuit(cv::Mat& suitImage) const -> Card::Suit override;

        private:
            [[nodiscard]] auto _cvColorToString(cv::Vec3b color) const -> std::string;
    };
}  // namespace OCR