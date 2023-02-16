#include "OcrInterface.hpp"

namespace OCR {
    using enum cv::text::ocr_engine_mode;
    using enum cv::text::page_seg_mode;

    OcrInterface::OcrInterface() {
        // DEFAULT datapath = "/usr/local/share/tessdata"
        _tesseractCard = cv::text::OCRTesseract::create(nullptr, "eng", "23456789TJQKA", OEM_CUBE_ONLY, PSM_SINGLE_CHAR);
        _tesseractWord = cv::text::OCRTesseract::create(nullptr, "eng", nullptr, OEM_CUBE_ONLY, PSM_SINGLE_WORD);
    }

    auto OCR::OcrInterface::readCard(cv::Mat& rankImage, cv::Mat& suitImage) const -> Card {
        return {readCardRank(rankImage), readCardSuit(suitImage)};
    }
}  // namespace OCR