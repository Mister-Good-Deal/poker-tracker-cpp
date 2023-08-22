#include "ocr/WinamaxOcr.hpp"

#include <logger/Logger.hpp>
#include <utilities/Image.hpp>
#include <utilities/Strings.hpp>

namespace OCR {
    using std::min;
    using Utilities::Strings::InvalidNumberException;

    using Logger = Logger::Quill;

    using enum GameHandler::Card::Suit;
    using enum ActionType;

    WinamaxOcr::WinamaxOcr() :
      _cardsSkin(cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/cards_skins/" + DEFAULT_CARD_SKIN)), _buttonImg(getButtonImg()),
      OcrInterface(CARD_WIDTH) {}

    auto WinamaxOcr::operator=(WinamaxOcr&& other) noexcept -> WinamaxOcr& {
        if (this != &other) {
            _cardsSkin = std::move(other._cardsSkin);
            _buttonImg = std::move(other._buttonImg);
        }

        return *this;
    }

    auto WinamaxOcr::readAverageStack(const cv::Mat& averageStackImage) const -> int32_t {
        try {
            return readIntNumbers(_trimSalt(_extractWhiteTextDarkBackground(*_resizedImage(averageStackImage, 150)), 75));
        } catch (const InvalidNumberException& e) { throw CannotReadAverageStackImageException(averageStackImage); }
    }

    auto WinamaxOcr::readBigBlind(const cv::Mat& blindRangeImage) const -> int32_t {
        try {
            return readBlindRange(blindRangeImage).BB();
        } catch (const InvalidNumberException& e) { throw CannotReadBigBlindImageException(blindRangeImage); }
    }

    auto WinamaxOcr::readBlindLevel(const cv::Mat& blindLevelImage) const -> int32_t {
        try {
            return readIntNumbers(blindLevelImage);
        } catch (const InvalidNumberException& e) { throw CannotReadBlindLevelImageException(blindLevelImage); }
    }

    auto WinamaxOcr::readBlindLevelDuration(const cv::Mat& durationImage) const -> seconds {
        try {
            return readDuration(durationImage);
        } catch (const InvalidNumberException& e) { throw CannotReadBlindLevelDurationImageException(durationImage); }
    }

    auto WinamaxOcr::readBlindRange(const cv::Mat& blindRangeImage) const -> Blinds {
        try {
            auto range = readIntRange(blindRangeImage);

            return {range.first, range.second};
        } catch (const InvalidNumberException& e) { throw CannotReadBlindRangeImageException(blindRangeImage); }
    }

    auto WinamaxOcr::readCardRank(const cv::Mat& rankImage) const -> Card::Rank {
        auto rank = _cardOcr()->run(rankImage, OCR_MIN_CONFIDENCE);

        return Card::charToRank(rank[0]);
    }

    auto WinamaxOcr::readCardSuit(const cv::Mat& suitImage) const -> Card::Suit {
        // OpenCV uses HSV values in the range: H: 0-180, S: 0-255, V: 0-255
        const cv::Vec3b HEART_COLOR   = {0, 255, 255};    // Red
        const cv::Vec3b DIAMOND_COLOR = {120, 255, 255};  // Blue
        const cv::Vec3b CLUB_COLOR    = {55, 255, 255};   // Green
        const cv::Vec3b SPADE_COLOR   = {0, 0, 0};        // Black

        const int32_t MAX_HUE_VALUE        = 180;  // The maximum Hue value in openCV color space
        const int32_t LUMINOSITY_THRESHOLD = 10;   // Lower will be darker
        const int32_t COLOR_THRESHOLD      = 17;   // This represents the hue 'cone'

        cv::Mat hsvImage;

        cv::cvtColor(suitImage, hsvImage, cv::COLOR_BGR2HSV);

        auto middlePixelColor = hsvImage.at<cv::Vec3b>(suitImage.cols / 2, suitImage.rows / 2);

        // Check the luminosity (V value)
        if (middlePixelColor[2] < LUMINOSITY_THRESHOLD) { return SPADE; }  // Black

        // Check the hue (H value) within the cylindrical HSV color model
        int hue = middlePixelColor[0];

        if (min(abs(hue - HEART_COLOR[0]), MAX_HUE_VALUE - abs(hue - HEART_COLOR[0])) < COLOR_THRESHOLD) {  // Red
            return HEART;
        } else if (min(abs(hue - DIAMOND_COLOR[0]), MAX_HUE_VALUE - abs(hue - DIAMOND_COLOR[0])) < COLOR_THRESHOLD) {  // Blue
            return DIAMOND;
        } else if (min(abs(hue - CLUB_COLOR[0]), MAX_HUE_VALUE - abs(hue - CLUB_COLOR[0])) < COLOR_THRESHOLD) {  // Green
            return CLUB;
        } else {
            throw UnknownCardSuitException(middlePixelColor, "HSV");
        }
    }

    auto WinamaxOcr::readGameAction(const cv::Mat& gameActionImage) const -> ActionType {
        auto action = readWord(_extractYellowText(*_resizedImage(gameActionImage)));

        LOG_DEBUG(Logger::getLogger(), "Text action read = {}", action);

        if (action == "CALL") {
            return CALL;
        } else if (action == "CHECK" || action == "MONTRE") {
            return CHECK;
        } else if (action == "FOLD") {
            return FOLD;
        } else if (action.starts_with("BET")) {
            return BET;
        } else if (action.starts_with("RAISES")) {
            return RAISE;
        } else if (action == "SMALL BLIND") {
            return PAY_SMALL_BLIND;
        } else if (action == "BIG BLIND") {
            return PAY_BIG_BLIND;
        } else {
            throw CannotReadGameActionImageException(gameActionImage, action);
        }
    }

    auto WinamaxOcr::readPlayerBet(const cv::Mat& playerBetImage) const -> int32_t {
        try {
            return readIntNumbers(_extractYellowText(*_resizedImage(playerBetImage)));
        } catch (const InvalidNumberException& e) { throw CannotReadPlayerBetImageException(playerBetImage); }
    }

    auto WinamaxOcr::readPlayerBetInBB(const cv::Mat& playerBetInBBImage) const -> double {
        try {
            return readFloatNumbers(_extractYellowText(*_resizedImage(playerBetInBBImage)));
        } catch (const InvalidNumberException& e) { throw CannotReadPlayerBetInBBImageException(playerBetInBBImage); }
    }

    auto WinamaxOcr::readPlayerName(const cv::Mat& playerNameImage) const -> std::string {
        auto playerName = readWordByChar(_applyDilation(_trimSalt(_extractWhiteTextLightBackground(*_resizedImage(playerNameImage)))));

        if (playerName.empty()) { throw CannotReadPlayerNameImageException(playerNameImage); }

        return playerName;
    }

    auto WinamaxOcr::readPot(const cv::Mat& potImage) const -> int32_t {
        try {
            return readIntNumbers(potImage);
        } catch (const InvalidNumberException& e) { throw CannotReadPotImageException(potImage); }
    }

    auto WinamaxOcr::readPotInBB(const cv::Mat& potInBBImage) const -> double {
        try {
            return readFloatNumbers(potInBBImage);
        } catch (const InvalidNumberException& e) { throw CannotReadPotInBBImageException(potInBBImage); }
    }

    auto WinamaxOcr::readPrizePool(const cv::Mat& prizePoolImage) const -> int32_t {
        try {
            return readIntNumbers(prizePoolImage);
        } catch (const InvalidNumberException& e) { throw CannotReadPrizePoolImageException(prizePoolImage); }
    }

    auto WinamaxOcr::readSmallBlind(const cv::Mat& blindRangeImage) const -> int32_t {
        try {
            return readBlindRange(blindRangeImage).SB();
        } catch (const CannotReadBlindRangeImageException& e) { throw CannotReadSmallBlindImageException(blindRangeImage); }
    }

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

    auto WinamaxOcr::_extractWhiteTextDarkBackground(cv::Mat& img) const -> cv::Mat& {
        // Light color range because winamax uses light colors (with a mix of different color) with a dark background
        return _colorRangeThreshold(img, {0, 0, 180}, {255, 255, 255});
    }

    auto WinamaxOcr::_extractWhiteTextLightBackground(cv::Mat& img) const -> cv::Mat& {
        return _colorRangeThreshold(img, {0, 0, 200}, {255, 55, 255});
    }

    auto WinamaxOcr::_extractYellowText(cv::Mat& img) const -> cv::Mat& {
        return _colorRangeThreshold(img, {20, 100, 100}, {30, 255, 255});
    }
    // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)

    auto WinamaxOcr::_colorRangeThreshold(cv::Mat& img, const cv::Scalar& colorLower, const cv::Scalar& colorUpper) const -> cv::Mat& {
        cv::Mat hsvImage, result;

        cv::cvtColor(img, hsvImage, cv::COLOR_BGR2HSV);
        cv::inRange(hsvImage, colorLower, colorUpper, img);

        return img;
    }

    auto WinamaxOcr::_resizedImage(const cv::Mat& src, int32_t desiredWidth) const -> uniqueMat_t {
        auto ratio     = static_cast<double>(desiredWidth) / src.cols;
        auto newHeight = static_cast<int32_t>(src.rows * ratio);

        cv::Mat resized;
        cv::resize(src, resized, cv::Size(desiredWidth, newHeight));

        return std::make_unique<cv::Mat>(resized);
    }

    auto WinamaxOcr::_trimSalt(cv::Mat& img, int32_t minClusterSize) const -> cv::Mat& {
        const int32_t LABEL_CONNECTIVITY = 8;

        // Find connected components of the "white" areas
        cv::Mat labelsMat;
        int32_t nLabels = cv::connectedComponents(img, labelsMat, LABEL_CONNECTIVITY, CV_32S);

        // Filter clusters with fewer than minClusterSize pixels
        for (int label = 1; label < nLabels; label++) {
            if (cv::countNonZero(labelsMat == label) < minClusterSize) { img.setTo(0, labelsMat == label); }
        }

        return img;
    }

    auto WinamaxOcr::_applyDilation(cv::Mat& img, int32_t kernelSize) const -> cv::Mat& {
        // Define the structuring element for dilation
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelSize, kernelSize));

        // Apply dilation
        cv::dilate(img, img, kernel);

        return img;
    }

}  // namespace OCR