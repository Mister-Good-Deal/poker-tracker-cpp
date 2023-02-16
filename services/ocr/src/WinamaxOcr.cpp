
#include "WinamaxOcr.hpp"

namespace OCR {
    using enum GameHandler::Card::Suit;

    auto WinamaxOcr::operator=(WinamaxOcr&& other) noexcept -> WinamaxOcr& {
        if (this != &other)
        {
            // @todo, prefer using var = std::move(...);
        }

        return *this;
    }

    auto WinamaxOcr::readCardRank(cv::Mat& rankImage) const -> Card::Rank {
        std::string rank;

        _tesseractCard->run(rankImage, rank);

        return Card::charToRank(rank[0]);
    }

    auto WinamaxOcr::readCardSuit(cv::Mat& suitImage) const -> Card::Suit {
        // BGR color format
        const cv::Vec3b HEART_COLOR   = {27, 6, 177};   // Red
        const cv::Vec3b DIAMOND_COLOR = {131, 49, 12};  // Blue
        const cv::Vec3b CLUB_COLOR    = {46, 138, 0};   // Green
        const cv::Vec3b SPADE_COLOR   = {0, 0, 0};      // Black

        Card::Suit suit;

        auto middlePixelColor = suitImage.at<cv::Vec3b>(suitImage.cols / 2, suitImage.rows / 2);

        if (middlePixelColor == HEART_COLOR)
        {
            suit = HEART;
        } else if (middlePixelColor == DIAMOND_COLOR) {
            suit = DIAMOND;
        } else if (middlePixelColor == CLUB_COLOR) {
            suit = CLUB;
        } else if (middlePixelColor == SPADE_COLOR) {
            suit = SPADE;
        } else {
            throw UndefinedCardSuit("The middle pixel color BGR" + _cvColorToString(middlePixelColor) + " does not match a suit");
        }

        return suit;
    }

    auto WinamaxOcr::_cvColorToString(cv::Vec3b color) const -> std::string {
        return "(" + std::to_string(color[0]) + ", " + std::to_string(color[1]) + ", " + std::to_string(color[2]) + ")";
    }
}  // namespace OCR