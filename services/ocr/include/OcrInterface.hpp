#pragma once

#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/text.hpp>

#include <Card.hpp>

namespace OCR {
    using cv::text::OCRTesseract;
    using GameHandler::Card;

    class OcrInterface {
        public:
            static constexpr double      SIMILARITY_THRESHOLD = 0.99;
            static constexpr const char* ALL_CHARACTERS       = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_ ";

            OcrInterface();
            OcrInterface(const OcrInterface&) = default;
            OcrInterface(OcrInterface&&)      = default;

            virtual ~OcrInterface() = default;

            auto operator=(const OcrInterface&) -> OcrInterface& = default;
            auto operator=(OcrInterface&&) -> OcrInterface&      = default;

            [[nodiscard]] virtual auto getRankCardArea() const -> cv::Rect = 0;
            [[nodiscard]] virtual auto getSuitCardArea() const -> cv::Rect = 0;
            [[nodiscard]] virtual auto getButtonImg() const -> cv::Mat     = 0;
            [[nodiscard]] virtual auto getButtonMask() const -> cv::Mat    = 0;

            virtual auto readCardRank(cv::Mat& rankImage) const -> Card::Rank          = 0;
            virtual auto readCardSuit(cv::Mat& suitImage) const -> Card::Suit          = 0;
            virtual auto readPlayerName(cv::Mat& playerNameImage) const -> std::string = 0;
            virtual auto readCard(cv::Mat& cardImage) const -> Card;
            virtual auto readWord(cv::Mat& wordImage) const -> std::string;
            virtual auto readWordByChar(cv::Mat& wordImage) const -> std::string;
            virtual auto readNumbers(cv::Mat& numberImage) const -> int32_t;

            [[nodiscard]] auto isSimilar(const cv::Mat& firstImage, const cv::Mat& secondImage,
                                         double threshold = SIMILARITY_THRESHOLD, cv::InputArray& mask = cv::noArray()) const -> bool;

        protected:
            [[nodiscard]] auto _cardOcr() const -> const cv::Ptr<OCRTesseract>& { return _tesseractCard; }
            [[nodiscard]] auto _charOcr() const -> const cv::Ptr<OCRTesseract>& { return _tesseractChar; }
            [[nodiscard]] auto _wordOcr() const -> const cv::Ptr<OCRTesseract>& { return _tesseractWord; }
            [[nodiscard]] auto _similarityScore(const cv::Mat& firstImage, const cv::Mat& secondImage,
                                                cv::InputArray& mask = cv::noArray()) const -> double;

        private:
            cv::Ptr<OCRTesseract> _tesseractCard;
            cv::Ptr<OCRTesseract> _tesseractWord;
            cv::Ptr<OCRTesseract> _tesseractChar;
            cv::Ptr<OCRTesseract> _tesseractNumbers;
    };

}  // namespace OCR
