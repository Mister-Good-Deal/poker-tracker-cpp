#include "OcrInterface.hpp"

#include <Logger.hpp>

namespace OCR {
    using Logger = Logger::Quill;

    using enum cv::text::ocr_engine_mode;
    using enum cv::text::page_seg_mode;

    namespace {
        static inline void ltrim(std::string& s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
        }

        static inline void rtrim(std::string& s) {
            s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
        }

        static inline void fullTrim(std::string& s) {
            s.erase(std::remove_if(s.begin(), s.end(), [](unsigned char ch) { return std::isspace(ch); }), s.end());
        }

        static inline void trim(std::string& s) {
            rtrim(s);
            ltrim(s);
        }
    }  // namespace

    OcrInterface::OcrInterface() {
        // DEFAULT datapath = "/usr/local/share/tessdata"
        _tesseractCard    = cv::text::OCRTesseract::create(nullptr, "eng", "23456789TJQKA", OEM_CUBE_ONLY, PSM_SINGLE_CHAR);
        _tesseractWord    = cv::text::OCRTesseract::create(nullptr, "eng", nullptr, OEM_CUBE_ONLY, PSM_SINGLE_WORD);
        _tesseractNumbers = cv::text::OCRTesseract::create(nullptr, "eng", "0123456789", OEM_CUBE_ONLY, PSM_SINGLE_WORD);
    }

    auto OCR::OcrInterface::readCard(cv::Mat& cardImage) const -> Card {
        cv::Mat rankImage = cardImage(getRankCardArea());
        cv::Mat suitImage = cardImage(getSuitCardArea());

        return {readCardRank(rankImage), readCardSuit(suitImage)};
    }

    auto OcrInterface::readWord(cv::Mat& wordImage) const -> std::string {
        std::string word;

        _tesseractWord->run(wordImage, word);

        trim(word);

        return word;
    }

    auto OcrInterface::readNumbers(cv::Mat& prizePoolImage) const -> int32_t {
        std::string word;

        _tesseractNumbers->run(prizePoolImage, word);

        fullTrim(word);

        return std::atoi(word.c_str());
    }

    auto OcrInterface::isSimilar(const cv::Mat& firstImage, const cv::Mat& secondImage, double threshold, cv::InputArray& mask) const
        -> bool {
        return _similarityScore(firstImage, secondImage, mask) >= threshold;
    }

    auto OcrInterface::_similarityScore(const cv::Mat& firstImage, const cv::Mat& secondImage, cv::InputArray& mask) const -> double {
        double similarity = 0;

        if (firstImage.rows != secondImage.rows)
        {
            LOG_ERROR(Logger::getLogger(), "The images rows are not equal in similarity images computation ({} != {})",
                      firstImage.rows, secondImage.rows);
        } else if (firstImage.cols != secondImage.cols) {
            LOG_ERROR(Logger::getLogger(), "The images cols are not equal in similarity images computation ({} != {})",
                      firstImage.cols, secondImage.cols);
        } else {
            double errorL2 = cv::norm(firstImage, secondImage, cv::NORM_L2, mask);

            similarity = errorL2 / static_cast<double>(firstImage.rows * firstImage.cols);
        }

        return similarity;
    }
}  // namespace OCR