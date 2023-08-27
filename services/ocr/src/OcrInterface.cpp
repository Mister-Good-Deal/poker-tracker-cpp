#include "ocr/OcrInterface.hpp"

#include <logger/Logger.hpp>
#include <utilities/Image.hpp>
#include <utilities/Strings.hpp>

namespace OCR {
    using Utilities::Strings::fullTrim;
    using Utilities::Strings::removeChar;
    using Utilities::Strings::replaceChar;
    using Utilities::Strings::toFloat;
    using Utilities::Strings::toInt;
    using Utilities::Strings::trim;
    using Utilities::Image::writeLearningImage;

    using Logger = Logger::Quill;

    using enum cv::text::ocr_engine_mode;
    using enum cv::text::page_seg_mode;

    OcrInterface::OcrInterface(int32_t cardWidth)
      : _cardWidth(cardWidth) {
        // DEFAULT datapath = "/usr/local/share/tessdata"
        _tesseractCard         = cv::text::OCRTesseract::create(nullptr, "eng", "0123456789TJQKA", OEM_CUBE_ONLY, PSM_SINGLE_CHAR);
        _tesseractWord         = cv::text::OCRTesseract::create(nullptr, "eng", ALL_CHARACTERS, OEM_CUBE_ONLY, PSM_SINGLE_BLOCK);
        _tesseractChar         = cv::text::OCRTesseract::create(nullptr, "eng", ALL_CHARACTERS, OEM_CUBE_ONLY, PSM_SINGLE_CHAR);
        _tesseractIntNumbers   = cv::text::OCRTesseract::create(nullptr, "eng", "0123456789 ", OEM_CUBE_ONLY, PSM_SINGLE_CHAR);
        _tesseractIntRange     = cv::text::OCRTesseract::create(nullptr, "eng", "0123456789- ", OEM_CUBE_ONLY, PSM_SINGLE_CHAR);
        _tesseractFloatNumbers = cv::text::OCRTesseract::create(nullptr, "eng", "0123456789,.B ", OEM_CUBE_ONLY, PSM_SINGLE_CHAR);
        _tesseractDuration     = cv::text::OCRTesseract::create(nullptr, "eng", "0123456789: ", OEM_CUBE_ONLY, PSM_SINGLE_CHAR);
    }

    auto OCR::OcrInterface::readBoardCard(const cv::Mat& cardImage) const -> Card {
        try {
            cv::Mat rankImage = cardImage(getRankCardArea());
            cv::Mat suitImage = cardImage(getSuitCardArea());
            
            Card card {readCardRank(rankImage), readCardSuit(suitImage)};
            
            writeLearningImage(cardImage, LEARNING_DATA_DIR, "card", fmt::format("{}", card));

            return card;
        } catch (const UnknownCardRankException& e) {
            throw CannotReadBoardCardRankImageException(e, cardImage);
        } catch (const UnknownCardSuitException& e) { throw CannotReadBoardCardSuitImageException(e, cardImage); }
    }

    auto OCR::OcrInterface::readPlayerCard(const cv::Mat& cardImage) const -> Card {
        try {
            cv::Mat rankImage = cardImage(getRankCardArea());
            cv::Mat suitImage = cardImage(getSuitCardArea());

            return {readCardRank(rankImage), readCardSuit(suitImage)};
        } catch (const UnknownCardRankException& e) {
            throw CannotReadPlayerCardRankImageException(e, cardImage);
        } catch (const UnknownCardSuitException& e) { throw CannotReadPlayerCardSuitImageException(e, cardImage); }
    }

    auto OcrInterface::readHand(const cv::Mat& handImage) const -> Hand {
        cv::Mat firstCardImage  = handImage({0, 0, _cardWidth, handImage.rows});
        cv::Mat secondCardImage = handImage({handImage.cols - _cardWidth, 0, _cardWidth, handImage.rows});
        
        Card firstCard = readPlayerCard(firstCardImage);
        Card secondCard = readPlayerCard(secondCardImage);
        
        writeLearningImage(handImage, LEARNING_DATA_DIR, "hand", fmt::format("{}-{}", firstCard, secondCard));

        return {firstCard, secondCard};
    }

    auto OcrInterface::readWord(const cv::Mat& wordImage) const -> std::string {
        auto word = _tesseractWord->run(wordImage, OCR_MIN_CONFIDENCE, cv::text::OCR_LEVEL_TEXTLINE);

        trim(word);

        return word;
    }

    auto OcrInterface::readWordByChar(const cv::Mat& wordImage) const -> std::string {
        auto word = _tesseractChar->run(wordImage, OCR_MIN_CONFIDENCE);

        trim(word);

        return word;
    }

    auto OcrInterface::readIntNumbers(const cv::Mat& intNumberImage) const -> int32_t {
        auto number = _tesseractIntNumbers->run(intNumberImage, OCR_MIN_CONFIDENCE);

        fullTrim(number);

        return toInt(number);
    }

    auto OcrInterface::readIntRange(const cv::Mat& intRangeImage) const -> intRange {
        auto range = _tesseractIntRange->run(intRangeImage, OCR_MIN_CONFIDENCE);

        fullTrim(range);

        auto dashPos      = range.find('-');
        auto firstNumber  = range.substr(0, dashPos);
        auto secondNumber = range.substr(dashPos + 1);

        return {toInt(firstNumber), toInt(secondNumber)};
    }

    auto OcrInterface::readFloatNumbers(const cv::Mat& floatNumberImage) const -> double {
        auto number = _tesseractFloatNumbers->run(floatNumberImage, OCR_MIN_CONFIDENCE);

        fullTrim(number);
        removeChar(number, 'B');
        replaceChar(number, ',', '.');

        return toFloat(number);
    }

    auto OcrInterface::readDuration(const cv::Mat& clockImage) const -> seconds {
        auto clock = _tesseractDuration->run(clockImage, OCR_MIN_CONFIDENCE);

        fullTrim(clock);

        auto doubleDashPos = clock.find(':');
        auto minutes       = clock.substr(0, doubleDashPos);
        auto seconds       = clock.substr(doubleDashPos + 1);

        return std::chrono::seconds(toInt(minutes) * 60 + toInt(seconds));
    }
}  // namespace OCR