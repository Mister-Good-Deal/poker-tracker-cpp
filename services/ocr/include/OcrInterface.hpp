#pragma once

#include "Card.hpp"
#include <opencv4/opencv2/opencv.hpp>

namespace OCR {
    using GameHandler::Card;

    class OcrInterface {
        public:
            OcrInterface(const OcrInterface&) = default;
            OcrInterface(OcrInterface&&)      = default;

            virtual ~OcrInterface() = default;

            auto operator=(const OcrInterface&) -> OcrInterface& = default;
            auto operator=(OcrInterface&&) -> OcrInterface&      = default;

            virtual auto readCard(const cv::Mat& image) -> Card        = 0;
            virtual auto readText(const cv::Mat& image) -> std::string = 0;
    };

}  // namespace OCR
