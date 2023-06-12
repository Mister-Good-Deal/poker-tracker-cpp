#include "WinamaxOcr.hpp"

#include <Logger.hpp>

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

    auto WinamaxOcr::readPlayerName(cv::Mat& playerNameImage) const -> std::string {
        _processImageBeforeWhiteTextOcr(playerNameImage);

        return readWordByChar(playerNameImage);
    }

    auto WinamaxOcr::readGameAction(cv::Mat& gameActionImage) const -> std::string {
        _processImageBeforeYellowTextOcr(gameActionImage);

        return readWord(gameActionImage);
    }

    auto WinamaxOcr::getButtonMask() const -> cv::Mat {
        auto buttonImg = getButtonImg();
        auto mask      = cv::Mat(buttonImg.rows, buttonImg.cols, CV_8UC1, cv::Scalar(0));

        cv::circle(mask, {buttonImg.rows / 2, buttonImg.cols / 2}, buttonImg.cols / 2, 1, -1);

        return mask;
    }

    auto WinamaxOcr::hasFolded(cv::Mat& handImage) const -> bool { return !isSimilar(handImage, _cardsSkin); }

    auto WinamaxOcr::_cvColorToString(const cv::Vec3b& color) const -> std::string {
        return "(" + std::to_string(color[0]) + ", " + std::to_string(color[1]) + ", " + std::to_string(color[2]) + ")";
    }

    auto WinamaxOcr::_processImageBeforeWhiteTextOcr(cv::Mat& image) const -> void {
        cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
        cv::threshold(image, image, 205, 255, cv::THRESH_BINARY);
    }

    auto WinamaxOcr::_processImageBeforeYellowTextOcr(cv::Mat& image) const -> void {
        _colorRangeThreshold(image, {22, 0, 0}, {42, 255, 255});
    }

    auto WinamaxOcr::_colorRangeThreshold(cv::Mat& image, const cv::Scalar& colorLower, const cv::Scalar& colorUpper) const -> void {
        cv::Mat hsvImage, colorMask;

        cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);
        cv::inRange(hsvImage, colorLower, colorUpper, image);
    }
}  // namespace OCR