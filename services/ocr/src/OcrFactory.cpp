#include "OcrFactory.hpp"

#include <WinamaxOcr.hpp>

namespace OCR::Factory {
    auto OcrFactory::create(const std::string& ocrName) -> std::unique_ptr<OcrInterface> {
        if (ocrName == "Winamax") { return std::make_unique<WinamaxOcr>(); }

        throw invalid_ocr("Invalid OCR name: " + ocrName);
    }
}  // namespace OCR::Factory