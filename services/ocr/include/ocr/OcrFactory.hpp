#pragma once

#include <ocr/OcrInterface.hpp>

namespace OCR::Factory {
    class invalid_ocr : public std::runtime_error {
        public:
            explicit invalid_ocr(const std::string& arg)
              : runtime_error(arg) {};
    };

    class OcrFactory {
        public:
            OcrFactory() = delete;

            static auto create(const std::string& ocrName) -> std::unique_ptr<OcrInterface>;
    };
}  // namespace OCR::Factory