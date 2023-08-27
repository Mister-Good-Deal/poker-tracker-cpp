#pragma once

#include <ocr/OcrInterface.hpp>

namespace OCR::Factory {
    class invalid_ocr : public std::invalid_argument {
        public:
            explicit invalid_ocr(const std::string& arg)
              : invalid_argument(arg) {};
    };

    class OcrFactory {
        public:
            OcrFactory() = delete;

            static auto create(const std::string& ocrName) -> std::unique_ptr<OcrInterface>;
    };
}  // namespace OCR::Factory