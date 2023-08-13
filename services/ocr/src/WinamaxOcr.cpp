#include "ocr/WinamaxOcr.hpp"

#include <logger/Logger.hpp>
#include <utilities/Macros.hpp>

namespace OCR {
    using Logger = Logger::Quill;

    using enum GameHandler::Card::Suit;

    WinamaxOcr::WinamaxOcr() :
        _cardsSkin(cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/cards_skins/" + DEFAULT_CARD_SKIN)), _buttonImg(getButtonImg()) {}

    auto WinamaxOcr::operator=(WinamaxOcr&& other) noexcept -> WinamaxOcr& {
        if (this != &other) {
            _cardsSkin = std::move(other._cardsSkin);
            _buttonImg = std::move(other._buttonImg);
        }

        return *this;
    }

    auto WinamaxOcr::readCardRank(const cv::Mat& rankImage) const -> Card::Rank {
        auto rank = _cardOcr()->run(rankImage, OCR_MIN_CONFIDENCE);

        return Card::charToRank(rank[0]);
    }

    auto WinamaxOcr::readCardSuit(const cv::Mat& suitImage) const -> Card::Suit {
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

        if (abs(middlePixelColorR - HEART_COLOR[2]) <= EPSILON) {
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

    auto WinamaxOcr::readAverageStack(const cv::Mat& averageStackImage) const -> int32_t {
        return readIntNumbers(_extractWhiteTextDarkBackground(averageStackImage));
    }

    auto WinamaxOcr::readBigBlind(const cv::Mat& blindRangeImage) const -> int32_t { return readBlindRange(blindRangeImage).BB(); }
    auto WinamaxOcr::readBlindLevel(const cv::Mat& blindLevelImage) const -> int32_t { return readIntNumbers(blindLevelImage); }
    auto WinamaxOcr::readBlindLevelDuration(const cv::Mat& durationImage) const -> seconds { return readDuration(durationImage); }

    auto WinamaxOcr::readBlindRange(const cv::Mat& blindRangeImage) const -> Blinds {
        auto range = readIntRange(blindRangeImage);

        return {range.first, range.second};
    }

    auto WinamaxOcr::readGameAction(const cv::Mat& gameActionImage) const -> std::string {
        return readWord(_extractYellowText(gameActionImage));
    }

    auto WinamaxOcr::readPlayerBet(const cv::Mat& playerBetImage) const -> int32_t {
        return readIntNumbers(_extractYellowText(playerBetImage));
    }

    auto WinamaxOcr::readPlayerBetInBB(const cv::Mat& playerBetInBBImage) const -> double {
        return readFloatNumbers(_extractYellowText(playerBetInBBImage));
    }

    auto WinamaxOcr::readPlayerName(const cv::Mat& playerNameImage) const -> std::string {
        return readWordByChar(_extractWhiteTextLightBackground(playerNameImage));
    }

    auto WinamaxOcr::readPot(const cv::Mat& potImage) const -> int32_t { return readIntNumbers(potImage); }
    auto WinamaxOcr::readPotInBB(const cv::Mat& potInBBImage) const -> double { return readFloatNumbers(potInBBImage); }
    auto WinamaxOcr::readPrizePool(const cv::Mat& prizePoolImage) const -> int32_t { return readIntNumbers(prizePoolImage); }
    auto WinamaxOcr::readSmallBlind(const cv::Mat& blindRangeImage) const -> int32_t { return readBlindRange(blindRangeImage).SB(); }

    auto WinamaxOcr::getButtonMask() const -> cv::Mat {
        auto buttonImg = getButtonImg();
        auto mask      = cv::Mat(buttonImg.rows, buttonImg.cols, CV_8UC1, cv::Scalar(0));

        cv::circle(mask, {buttonImg.rows / 2, buttonImg.cols / 2}, buttonImg.cols / 2, 255, -1);

        return mask;
    }

    auto WinamaxOcr::getButtonImg() const -> cv::Mat { return cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/" + DEFAULT_BUTTON_IMG); }
    auto WinamaxOcr::hasFolded(const cv::Mat& handImage) const -> bool { return !isSimilar(handImage, _cardsSkin); }

    // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
    auto WinamaxOcr::getRankCardArea() const -> cv::Rect { return {0, 0, 20, 23}; }
    auto WinamaxOcr::getSuitCardArea() const -> cv::Rect { return {2, 25, 14, 16}; }

    auto WinamaxOcr::hasButton(const cv::Mat& buttonImage) const -> bool {
        return isSimilar(buttonImage, getButtonImg(), 0.05, getButtonMask());
    }

    auto WinamaxOcr::_extractWhiteTextDarkBackground(const cv::Mat& image) const -> cv::Mat {
        // Light color range because winamax uses light colors (with a mix of different color) with a dark background
        return _colorRangeThreshold(image, {0, 0, 180}, {255, 255, 255});
    }

    auto WinamaxOcr::_extractWhiteTextLightBackground(const cv::Mat& image) const -> cv::Mat {
        return _colorRangeThreshold(image, {0, 0, 200}, {255, 55, 255});
    }

    auto WinamaxOcr::_extractYellowText(const cv::Mat& image) const -> cv::Mat {
        return _colorRangeThreshold(image, {22, 0, 0}, {42, 255, 255});
    }
    // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)

    auto WinamaxOcr::_cvColorToString(const cv::Vec3b& color) const -> std::string {
        return "(" + std::to_string(color[0]) + ", " + std::to_string(color[1]) + ", " + std::to_string(color[2]) + ")";
    }

    auto WinamaxOcr::_colorRangeThreshold(const cv::Mat& image, const cv::Scalar& colorLower, const cv::Scalar& colorUpper) const
        -> cv::Mat {
        cv::Mat hsvImage, result;

        DISPLAY_IMAGE("colorRangeThreshold input", image);

        cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);
        cv::inRange(hsvImage, colorLower, colorUpper, result);

        DISPLAY_IMAGE("colorRangeThreshold output", result);

        return result;
    }
}  // namespace OCR