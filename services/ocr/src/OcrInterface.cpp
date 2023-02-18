#include "OcrInterface.hpp"

namespace OCR {
    using enum cv::text::ocr_engine_mode;
    using enum cv::text::page_seg_mode;

    namespace {
        static inline void ltrim(std::string& s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
        }

        static inline void rtrim(std::string& s) {
            s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
        }

        static inline void trim(std::string& s) {
            rtrim(s);
            ltrim(s);
        }
    }  // namespace

    OcrInterface::OcrInterface() {
        // DEFAULT datapath = "/usr/local/share/tessdata"
        _tesseractCard = cv::text::OCRTesseract::create(nullptr, "eng", "23456789TJQKA", OEM_CUBE_ONLY, PSM_SINGLE_CHAR);
        _tesseractWord = cv::text::OCRTesseract::create(nullptr, "eng", nullptr, OEM_CUBE_ONLY, PSM_SINGLE_WORD);
    }

    auto OCR::OcrInterface::readCard(cv::Mat& rankImage, cv::Mat& suitImage) const -> Card {
        return {readCardRank(rankImage), readCardSuit(suitImage)};
    }

    auto OcrInterface::readWord(cv::Mat& wordImage) const -> std::string {
        std::string word;

        _tesseractWord->run(wordImage, word);

        trim(word);

        return word;
    }
}  // namespace OCR