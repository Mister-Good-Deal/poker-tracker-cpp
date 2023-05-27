#pragma once

#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/text.hpp>

#include <Card.hpp>

namespace OCR {
    using cv::text::OCRTesseract;
    using GameHandler::Card;

    class OcrInterface {
        public:
            OcrInterface();
            OcrInterface(const OcrInterface&) = default;
            OcrInterface(OcrInterface&&)      = default;

            virtual ~OcrInterface() = default;

            auto operator=(const OcrInterface&) -> OcrInterface& = default;
            auto operator=(OcrInterface&&) -> OcrInterface&      = default;

            [[nodiscard]] virtual auto getRankCardArea() const -> cv::Rect = 0;
            [[nodiscard]] virtual auto getSuitCardArea() const -> cv::Rect = 0;

            virtual auto readCardRank(cv::Mat& rankImage) const -> Card::Rank = 0;
            virtual auto readCardSuit(cv::Mat& suitImage) const -> Card::Suit = 0;
            virtual auto readCard(cv::Mat& cardImage) const -> Card;
            virtual auto readWord(cv::Mat& wordImage) const -> std::string;
            virtual auto readPrizePool(cv::Mat& wordImage) const -> int32_t;

            [[nodiscard]] virtual auto similarityScore(const cv::Mat& firstImage, const cv::Mat& secondImage) const -> double;

        protected:
            [[nodiscard]] auto _cardOcr() const -> const cv::Ptr<OCRTesseract>& { return _tesseractCard; }
            [[nodiscard]] auto _wordOcr() const -> const cv::Ptr<OCRTesseract>& { return _tesseractWord; }

        private:
            cv::Ptr<OCRTesseract> _tesseractCard;
            cv::Ptr<OCRTesseract> _tesseractWord;
            cv::Ptr<OCRTesseract> _tesseractNumbers;
    };

}  // namespace OCR
