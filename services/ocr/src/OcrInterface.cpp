#include "OcrInterface.hpp"

#include <charconv>

#include <Logger.hpp>

namespace OCR {
    using Logger = Logger::Quill;

    using enum cv::text::ocr_engine_mode;
    using enum cv::text::page_seg_mode;

    namespace {
        static inline auto constexpr ltrim(std::string& s) -> void {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
        }

        static inline auto constexpr rtrim(std::string& s) -> void {
            s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
        }

        static inline auto constexpr fullTrim(std::string& s) -> void {
            s.erase(std::remove_if(s.begin(), s.end(), [](unsigned char ch) { return std::isspace(ch); }), s.end());
        }

        static inline auto constexpr removeChar(std::string& s, char toRemove) -> void {
            s.erase(std::remove(s.begin(), s.end(), toRemove), s.end());
        }

        static inline auto constexpr replaceChar(std::string& s, char find, char replace) -> void {
            std::replace(s.begin(), s.end(), find, replace);
        }

        static inline auto constexpr trim(std::string& s) -> void {
            rtrim(s);
            ltrim(s);
        }

        static inline auto constexpr toFloat(std::string_view s) -> double {
            double value = 0.0;

            auto error = std::from_chars(s.begin(), s.end(), value);

            if (error.ec != std::errc()) { throw std::runtime_error("Could not convert string to float"); }

            return value;
        }
    }  // namespace

    OcrInterface::OcrInterface() {
        // DEFAULT datapath = "/usr/local/share/tessdata"
        _tesseractCard        = cv::text::OCRTesseract::create(nullptr, "eng", "23456789TJQKA", OEM_CUBE_ONLY, PSM_SINGLE_CHAR);
        _tesseractWord        = cv::text::OCRTesseract::create(nullptr, "eng", ALL_CHARACTERS, OEM_CUBE_ONLY, PSM_SINGLE_BLOCK);
        _tesseractChar        = cv::text::OCRTesseract::create(nullptr, "eng", ALL_CHARACTERS, OEM_CUBE_ONLY, PSM_SINGLE_CHAR);
        _tesseractNumbers     = cv::text::OCRTesseract::create(nullptr, "eng", "0123456789, ", OEM_CUBE_ONLY, PSM_SINGLE_CHAR);
        _tesseractNumbersInBB = cv::text::OCRTesseract::create(nullptr, "eng", "0123456789,B ", OEM_CUBE_ONLY, PSM_SINGLE_CHAR);
    }

    auto OCR::OcrInterface::readCard(const cv::Mat& cardImage) const -> Card {
        cv::Mat rankImage = cardImage(getRankCardArea());
        cv::Mat suitImage = cardImage(getSuitCardArea());

        return {readCardRank(rankImage), readCardSuit(suitImage)};
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

    auto OcrInterface::readNumbersInBB(const cv::Mat& numberInBBImage) const -> double {
        auto numberInBB = _tesseractNumbersInBB->run(numberInBBImage, OCR_MIN_CONFIDENCE);

        fullTrim(numberInBB);
        removeChar(numberInBB, 'B');
        replaceChar(numberInBB, ',', '.');

        return toFloat(numberInBB);
    }

    auto OcrInterface::readNumbers(const cv::Mat& numberImage) const -> double {
        auto number = _tesseractNumbers->run(numberImage, OCR_MIN_CONFIDENCE);

        fullTrim(number);
        replaceChar(number, ',', '.');

        return toFloat(number);
    }

    auto OcrInterface::isSimilar(const cv::Mat& firstImage, const cv::Mat& secondImage, double threshold, cv::InputArray& mask) const
        -> bool {
        return _similarityScore(firstImage, secondImage, mask) <= threshold;
    }

    auto OcrInterface::_cropCentered(cv::Mat& firstImage, cv::Mat& secondImage) const -> void {
        // Determine smaller image, then crop the bigger one to the size of the smaller one. First image is the bigger one.
        if (secondImage.cols > firstImage.cols || secondImage.rows > firstImage.rows) { std::swap(firstImage, secondImage); }

        if (firstImage.cols < secondImage.cols || firstImage.rows < secondImage.rows)
        { throw std::runtime_error("The first image must be bigger than the second one."); }

        auto colsBorder = (firstImage.cols - secondImage.cols) / 2;
        auto rowsBorder = (firstImage.rows - secondImage.rows) / 2;

        firstImage(cv::Rect(colsBorder, rowsBorder, secondImage.cols, secondImage.rows)).copyTo(firstImage);
    }

    /**
     * The lower the score, the more similar the images are.
     *
     * @todo Use a better crop method (shape detection matching to center the crop).
     *
     * @param firstImage The first image to compare.
     * @param secondImage The second image to compare.
     * @param mask The mask to apply to the images.
     *
     * @return The similarity score.
     */
    auto OcrInterface::_similarityScore(const cv::Mat& firstImage, const cv::Mat& secondImage, cv::InputArray& mask) const -> double {
        cv::Mat firstImageCopy  = firstImage;
        cv::Mat secondImageCopy = secondImage;

        if (firstImage.rows != secondImage.rows || firstImage.cols != secondImage.cols)
        {
            LOG_DEBUG(Logger::getLogger(),
                      "The images size are not equals in similarity images computation ({}x{} != {}x{}), cropping the bigger one.",
                      firstImage.rows, firstImage.cols, secondImage.rows, secondImage.cols);

            firstImageCopy  = firstImage.clone();
            secondImageCopy = secondImage.clone();
            _cropCentered(firstImageCopy, secondImageCopy);
        }

        double similarity = cv::norm(firstImageCopy, secondImageCopy, cv::NORM_RELATIVE | cv::NORM_L2SQR, mask);

        LOG_DEBUG(Logger::getLogger(), "Similarity score: {}", similarity);

        return similarity;
    }
}  // namespace OCR