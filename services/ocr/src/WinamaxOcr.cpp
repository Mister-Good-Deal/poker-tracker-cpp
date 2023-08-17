#include "ocr/WinamaxOcr.hpp"

#include <logger/Logger.hpp>
#include <utilities/Image.hpp>
#include <utilities/Strings.hpp>

namespace OCR {
    using Utilities::Strings::InvalidNumberException;

    using Logger = Logger::Quill;

    using enum GameHandler::Card::Suit;
    using enum ActionType;

    WinamaxOcr::WinamaxOcr() :
        _cardsSkin(cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/cards_skins/" + DEFAULT_CARD_SKIN)), _buttonImg(getButtonImg()) {}

    auto WinamaxOcr::operator=(WinamaxOcr&& other) noexcept -> WinamaxOcr& {
        if (this != &other) {
            _cardsSkin = std::move(other._cardsSkin);
            _buttonImg = std::move(other._buttonImg);
        }

        return *this;
    }

    auto WinamaxOcr::readAverageStack(const cv::Mat& averageStackImage) const -> int32_t {
        try {
            return readIntNumbers(_extractWhiteTextDarkBackground(averageStackImage));
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
        try {
            auto rank = _cardOcr()->run(rankImage, OCR_MIN_CONFIDENCE);
            return Card::charToRank(rank[0]);
        } catch (const UnknownCardRankException& e) { throw CannotReadCardRankImageException(e, rankImage); }
    }

    auto WinamaxOcr::readCardSuit(const cv::Mat& suitImage) const -> Card::Suit {
        const int32_t COLOR_INTENSITY_THRESHOLD = 130;
        const auto    BLACK_INTENSITY_THRESHOLD = std::make_tuple(20, 20, 20);
        // Winamax BGR color format
        const cv::Vec3i HEART_COLOR   = {27, 6, 177};   // Red
        const cv::Vec3i DIAMOND_COLOR = {131, 49, 12};  // Blue
        const cv::Vec3i CLUB_COLOR    = {46, 138, 0};   // Green
        const cv::Vec3i SPADE_COLOR   = {0, 0, 0};      // Black

        auto middlePixelColor  = suitImage.at<cv::Vec3b>(suitImage.cols / 2, suitImage.rows / 2);
        auto middlePixelColorB = static_cast<int32_t>(middlePixelColor[0]);
        auto middlePixelColorG = static_cast<int32_t>(middlePixelColor[1]);
        auto middlePixelColorR = static_cast<int32_t>(middlePixelColor[2]);

        if (middlePixelColorR >= COLOR_INTENSITY_THRESHOLD) {
            return HEART;
        } else if (middlePixelColorB >= COLOR_INTENSITY_THRESHOLD) {
            return DIAMOND;
        } else if (middlePixelColorG >= COLOR_INTENSITY_THRESHOLD) {
            return CLUB;
        } else if (std::tie(middlePixelColorB, middlePixelColorG, middlePixelColorR) <= BLACK_INTENSITY_THRESHOLD) {
            return SPADE;
        } else {
            throw CannotReadCardSuitImageException(suitImage);
        }
    }

    auto WinamaxOcr::readGameAction(const cv::Mat& gameActionImage) const -> ActionType {
        auto action = readWord(_extractYellowText(gameActionImage));

        LOG_DEBUG(Logger::getLogger(), "Text action read = {}", action);

        if (action == "CALL") {
            return CALL;
        } else if (action == "CHECK") {
            return CHECK;
        } else if (action == "FOLD") {
            return FOLD;
        } else if (action.starts_with("BET")) {
            return BET;
        } else if (action.starts_with("RAISES")) {
            return RAISE;
        } else if (action.starts_with("SMALL")) {
            return PAY_SMALL_BLIND;
        } else if (action.starts_with("BIG")) {
            // @fixme Read BIG BUND instead of BIG BLIND, L and I are merged into a U
            return PAY_BIG_BLIND;
        } else {
            throw CannotReadGameActionImageException(gameActionImage, action);
        }
    }

    auto WinamaxOcr::readPlayerBet(const cv::Mat& playerBetImage) const -> int32_t {
        try {
            return readIntNumbers(_extractYellowText(playerBetImage));
        } catch (const InvalidNumberException& e) { throw CannotReadPlayerBetImageException(playerBetImage); }
    }

    auto WinamaxOcr::readPlayerBetInBB(const cv::Mat& playerBetInBBImage) const -> double {
        try {
            return readFloatNumbers(_extractYellowText(playerBetInBBImage));
        } catch (const InvalidNumberException& e) { throw CannotReadPlayerBetInBBImageException(playerBetInBBImage); }
    }

    auto WinamaxOcr::readPlayerName(const cv::Mat& playerNameImage) const -> std::string {
        auto playerName = readWordByChar(_extractWhiteTextLightBackground(playerNameImage));

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

    auto WinamaxOcr::_colorRangeThreshold(const cv::Mat& image, const cv::Scalar& colorLower, const cv::Scalar& colorUpper) const
        -> cv::Mat {
        cv::Mat hsvImage, result;

        cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);
        cv::inRange(hsvImage, colorLower, colorUpper, result);

        return result;
    }
}  // namespace OCR