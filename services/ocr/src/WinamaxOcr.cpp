#include "WinamaxOcr.hpp"

#include "Logger.hpp"

namespace OCR {
    using Logger = Logger::Quill;

    using enum GameHandler::Card::Suit;

    auto WinamaxOcr::operator=(WinamaxOcr&& other) noexcept -> WinamaxOcr& {
        if (this != &other) { _cardsSkin = std::move(other._cardsSkin); }

        return *this;
    }

    auto WinamaxOcr::readCardRank(cv::Mat& rankImage) const -> Card::Rank {
        std::string rank;

        _cardOcr()->run(rankImage, rank);

        return Card::charToRank(rank[0]);
    }

    auto WinamaxOcr::readCardSuit(cv::Mat& suitImage) const -> Card::Suit {
        const int32_t EPSILON = 10;
        // Winamax BGR color format
        const cv::Vec3i HEART_COLOR   = {27, 6, 177};   // Red
        const cv::Vec3i DIAMOND_COLOR = {131, 49, 12};  // Blue
        const cv::Vec3i CLUB_COLOR    = {46, 138, 0};   // Green
        const cv::Vec3i SPADE_COLOR   = {0, 0, 0};      // Black

        Card::Suit suit = UNKNOWN;

        auto middlePixelColor  = suitImage.at<cv::Vec3b>(suitImage.cols / 2, suitImage.rows / 2);
        auto middlePixelColorB = static_cast<int32_t>(middlePixelColor[0]);
        auto middlePixelColorG = static_cast<int32_t>(middlePixelColor[1]);
        auto middlePixelColorR = static_cast<int32_t>(middlePixelColor[2]);

        if (abs(middlePixelColorR - HEART_COLOR[2]) <= EPSILON)
        {
            suit = HEART;
        } else if (abs(middlePixelColorB - DIAMOND_COLOR[0]) <= EPSILON) {
            suit = DIAMOND;
        } else if (abs(middlePixelColorG - CLUB_COLOR[1]) <= EPSILON) {
            suit = CLUB;
        } else if (middlePixelColorB <= EPSILON && middlePixelColorG <= EPSILON && middlePixelColorR <= EPSILON) {
            suit = SPADE;
        } else {
            LOG_ERROR(Logger::getLogger(), "The middle pixel color BGR{}, does not match a suit", _cvColorToString(middlePixelColor));
        }

        return suit;
    }

    auto WinamaxOcr::hasFolded(cv::Mat& cardsSkinImage) const -> bool {
        double errorL2    = cv::norm(cardsSkinImage, _cardsSkin, cv::NORM_L2);
        double similarity = errorL2 / static_cast<double>(cardsSkinImage.rows * cardsSkinImage.cols);

        return similarity >= SIMILARITY_THRESHOLD;
    }

    auto WinamaxOcr::_cvColorToString(const cv::Vec3b& color) const -> std::string {
        return "(" + std::to_string(color[0]) + ", " + std::to_string(color[1]) + ", " + std::to_string(color[2]) + ")";
    }
}  // namespace OCR