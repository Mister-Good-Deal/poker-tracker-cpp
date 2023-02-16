#include "WinamaxOcr.hpp"

#include "Logger.hpp"

namespace OCR {
    using Logger = Logger::Quill;

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

        _cardOcr()->run(rankImage, rank);

        return Card::charToRank(rank[0]);
    }

    auto WinamaxOcr::readCardSuit(cv::Mat& suitImage) const -> Card::Suit {
        // BGR color format
        const cv::Vec3b HEART_COLOR   = {27, 6, 177};   // Red
        const cv::Vec3b DIAMOND_COLOR = {131, 49, 12};  // Blue
        const cv::Vec3b CLUB_COLOR    = {46, 138, 0};   // Green
        const cv::Vec3b SPADE_COLOR   = {0, 0, 0};      // Black

        Card::Suit suit = UNKNOWN;

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
            LOG_ERROR(Logger::getLogger(), "The middle pixel color BGR{}, does not match a suit", _cvColorToString(middlePixelColor));
        }

        return suit;
    }

    auto WinamaxOcr::_cvColorToString(cv::Vec3b color) const -> std::string {
        return "(" + std::to_string(color[0]) + ", " + std::to_string(color[1]) + ", " + std::to_string(color[2]) + ")";
    }
}  // namespace OCR