#pragma once

#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/text.hpp>

#include <Card.hpp>

namespace OCR {
    using cv::text::OCRTesseract;
    using GameHandler::Card;
    using std::chrono::seconds;

    using intRange = std::pair<int32_t, int32_t>;

    class OcrInterface {
        public:
            static constexpr double      SIMILARITY_THRESHOLD = 0.05;
            static constexpr int32_t     OCR_MIN_CONFIDENCE   = 30;  // @todo confidence between numbers 1 and 7 is really low
            static constexpr const char* ALL_CHARACTERS       = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_ ";

            OcrInterface();
            OcrInterface(const OcrInterface&) = default;
            OcrInterface(OcrInterface&&)      = default;

            virtual ~OcrInterface() = default;

            auto operator=(const OcrInterface&) -> OcrInterface& = default;
            auto operator=(OcrInterface&&) -> OcrInterface&      = default;

            [[nodiscard]] virtual auto getRankCardArea() const -> cv::Rect                                 = 0;
            [[nodiscard]] virtual auto getSuitCardArea() const -> cv::Rect                                 = 0;
            [[nodiscard]] virtual auto getButtonImg() const -> cv::Mat                                     = 0;
            [[nodiscard]] virtual auto getButtonMask() const -> cv::Mat                                    = 0;
            [[nodiscard]] virtual auto readCardRank(const cv::Mat& rankImage) const -> Card::Rank          = 0;
            [[nodiscard]] virtual auto readCardSuit(const cv::Mat& suitImage) const -> Card::Suit          = 0;
            [[nodiscard]] virtual auto readPlayerName(const cv::Mat& playerNameImage) const -> std::string = 0;
            [[nodiscard]] virtual auto readCard(const cv::Mat& cardImage) const -> Card;
            [[nodiscard]] virtual auto readWord(const cv::Mat& wordImage) const -> std::string;
            [[nodiscard]] virtual auto readWordByChar(const cv::Mat& wordImage) const -> std::string;
            [[nodiscard]] virtual auto readIntNumbers(const cv::Mat& intNumberImage) const -> int32_t;
            [[nodiscard]] virtual auto readIntRange(const cv::Mat& intRangeImage) const -> intRange;
            [[nodiscard]] virtual auto readFloatNumbers(const cv::Mat& floatNumberImage) const -> double;
            [[nodiscard]] virtual auto readDuration(const cv::Mat& clockImage) const -> seconds;
            [[nodiscard]] auto         isSimilar(const cv::Mat& firstImage, const cv::Mat& secondImage,
                                                 double threshold = SIMILARITY_THRESHOLD, cv::InputArray& mask = cv::noArray()) const -> bool;

        protected:
            auto _cropCentered(cv::Mat& firstImage, cv::Mat& secondImage) const -> void;

            [[nodiscard]] auto _cardOcr() const -> const cv::Ptr<OCRTesseract>& { return _tesseractCard; }
            [[nodiscard]] auto _similarityScore(const cv::Mat& firstImage, const cv::Mat& secondImage,
                                                cv::InputArray& mask = cv::noArray()) const -> double;

        private:
            cv::Ptr<OCRTesseract> _tesseractCard;
            cv::Ptr<OCRTesseract> _tesseractWord;
            cv::Ptr<OCRTesseract> _tesseractChar;
            cv::Ptr<OCRTesseract> _tesseractIntNumbers;
            cv::Ptr<OCRTesseract> _tesseractIntRange;
            cv::Ptr<OCRTesseract> _tesseractFloatNumbers;
            cv::Ptr<OCRTesseract> _tesseractDuration;
    };

}  // namespace OCR
