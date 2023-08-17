#pragma once

#include <utility>

#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/text.hpp>

#include <game_handler/Card.hpp>
#include <game_handler/Round.hpp>

namespace OCR {
    using cv::text::OCRTesseract;
    using GameHandler::Blinds;
    using GameHandler::Card;
    using GameHandler::RoundAction;
    using GameHandler::UnknownCardRankException;
    using std::chrono::seconds;

    using ActionType = RoundAction::ActionType;
    using intRange   = std::pair<int32_t, int32_t>;

    class ExceptionWithImage : public std::exception {
        public:
            explicit ExceptionWithImage(const cv::Mat& image, std::string_view category) :
                _image(image.clone()), _category(category) {}

            [[nodiscard]] auto getImage() const noexcept -> const cv::Mat& { return _image; }
            [[nodiscard]] auto getCategory() const noexcept -> const std::string& { return _category; }

        private:
            cv::Mat     _image;  // The image is cloned to keep it alive after the exception is thrown
            std::string _category;
    };

    class CannotReadAverageStackImageException : public ExceptionWithImage {
        public:
            explicit CannotReadAverageStackImageException(const cv::Mat& image) : ExceptionWithImage(image, "averageStack") {}

            [[nodiscard]] auto what() const noexcept -> const char* override { return "Cannot read average stack"; }
    };

    class CannotReadBigBlindImageException : public ExceptionWithImage {
        public:
            explicit CannotReadBigBlindImageException(const cv::Mat& image) : ExceptionWithImage(image, "bigBlind") {}

            [[nodiscard]] auto what() const noexcept -> const char* override { return "Cannot read big blind"; }
    };

    class CannotReadBlindLevelImageException : public ExceptionWithImage {
        public:
            explicit CannotReadBlindLevelImageException(const cv::Mat& image) : ExceptionWithImage(image, "blindLevel") {}

            [[nodiscard]] auto what() const noexcept -> const char* override { return "Cannot read blind level"; }
    };

    class CannotReadBlindLevelDurationImageException : public ExceptionWithImage {
        public:
            explicit CannotReadBlindLevelDurationImageException(const cv::Mat& image) : ExceptionWithImage(image, "levelDuration") {}

            [[nodiscard]] auto what() const noexcept -> const char* override { return "Cannot read blind level duration"; }
    };

    class CannotReadBlindRangeImageException : public ExceptionWithImage {
        public:
            explicit CannotReadBlindRangeImageException(const cv::Mat& image) : ExceptionWithImage(image, "blindRange") {}

            [[nodiscard]] auto what() const noexcept -> const char* override { return "Cannot read blind range"; }
    };

    class CannotReadCardRankImageException : public ExceptionWithImage {
        public:
            explicit CannotReadCardRankImageException(UnknownCardRankException exception, const cv::Mat& image) :
                ExceptionWithImage(image, "cardRank"), _exception(std::move(exception)) {}

            [[nodiscard]] auto what() const noexcept -> const char* override { return "Cannot read card rank"; }

        private:
            UnknownCardRankException _exception;
    };

    class CannotReadCardSuitImageException : public ExceptionWithImage {
        public:
            explicit CannotReadCardSuitImageException(const cv::Mat& image) : ExceptionWithImage(image, "cardSuit") {}

            [[nodiscard]] auto what() const noexcept -> const char* override { return "Cannot read card suit"; }
    };

    class CannotReadGameActionImageException : public ExceptionWithImage {
        public:
            explicit CannotReadGameActionImageException(const cv::Mat& image, std::string_view action) :
                ExceptionWithImage(image, "gameAction"), _action(action) {}

            [[nodiscard]] auto what() const noexcept -> const char* override { return "Cannot read game action"; }

        private:
            std::string _action;
    };

    class CannotReadPlayerBetImageException : public ExceptionWithImage {
        public:
            explicit CannotReadPlayerBetImageException(const cv::Mat& image) : ExceptionWithImage(image, "playerBet") {}

            [[nodiscard]] auto what() const noexcept -> const char* override { return "Cannot read player bet"; }
    };

    class CannotReadPlayerBetInBBImageException : public ExceptionWithImage {
        public:
            explicit CannotReadPlayerBetInBBImageException(const cv::Mat& image) : ExceptionWithImage(image, "playerBetInBB") {}

            [[nodiscard]] auto what() const noexcept -> const char* override { return "Cannot read player bet in BB"; }
    };

    class CannotReadPlayerNameImageException : public ExceptionWithImage {
        public:
            explicit CannotReadPlayerNameImageException(const cv::Mat& image) : ExceptionWithImage(image, "playerName") {}

            [[nodiscard]] auto what() const noexcept -> const char* override { return "Cannot read player name"; }
    };

    class CannotReadPotImageException : public ExceptionWithImage {
        public:
            explicit CannotReadPotImageException(const cv::Mat& image) : ExceptionWithImage(image, "pot") {}

            [[nodiscard]] auto what() const noexcept -> const char* override { return "Cannot read pot"; }
    };

    class CannotReadPotInBBImageException : public ExceptionWithImage {
        public:
            explicit CannotReadPotInBBImageException(const cv::Mat& image) : ExceptionWithImage(image, "potInBB") {}

            [[nodiscard]] auto what() const noexcept -> const char* override { return "Cannot read pot in BB"; }
    };

    class CannotReadPrizePoolImageException : public ExceptionWithImage {
        public:
            explicit CannotReadPrizePoolImageException(const cv::Mat& image) : ExceptionWithImage(image, "prizePool") {}

            [[nodiscard]] auto what() const noexcept -> const char* override { return "Cannot read prize pool"; }
    };

    class CannotReadSmallBlindImageException : public ExceptionWithImage {
        public:
            explicit CannotReadSmallBlindImageException(const cv::Mat& image) : ExceptionWithImage(image, "smallBlind") {}

            [[nodiscard]] auto what() const noexcept -> const char* override { return "Cannot read small blind"; }
    };

    class OcrInterface {
        public:
            static constexpr double      SIMILARITY_THRESHOLD = 0.15;
            static constexpr int32_t     OCR_MIN_CONFIDENCE   = 30;  // @todo confidence between numbers 1 and 7 is really low
            static constexpr const char* ALL_CHARACTERS       = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_ ";

            OcrInterface();
            OcrInterface(const OcrInterface&) = default;
            OcrInterface(OcrInterface&&)      = default;

            virtual ~OcrInterface() = default;

            auto operator=(const OcrInterface&) -> OcrInterface& = default;
            auto operator=(OcrInterface&&) -> OcrInterface&      = default;

            [[nodiscard]] virtual auto getRankCardArea() const -> cv::Rect                                   = 0;
            [[nodiscard]] virtual auto getSuitCardArea() const -> cv::Rect                                   = 0;
            [[nodiscard]] virtual auto getButtonImg() const -> cv::Mat                                       = 0;
            [[nodiscard]] virtual auto getButtonMask() const -> cv::Mat                                      = 0;
            [[nodiscard]] virtual auto readAverageStack(const cv::Mat& averageBlindImage) const -> int32_t   = 0;
            [[nodiscard]] virtual auto readBigBlind(const cv::Mat& blindRangeImage) const -> int32_t         = 0;
            [[nodiscard]] virtual auto readBlindLevel(const cv::Mat& blindLevelImage) const -> int32_t       = 0;
            [[nodiscard]] virtual auto readBlindLevelDuration(const cv::Mat& durationImage) const -> seconds = 0;
            [[nodiscard]] virtual auto readBlindRange(const cv::Mat& blindRangeImage) const -> Blinds        = 0;
            [[nodiscard]] virtual auto readCardRank(const cv::Mat& rankImage) const -> Card::Rank            = 0;
            [[nodiscard]] virtual auto readCardSuit(const cv::Mat& suitImage) const -> Card::Suit            = 0;
            [[nodiscard]] virtual auto readGameAction(const cv::Mat& actionImage) const -> ActionType        = 0;
            [[nodiscard]] virtual auto readPlayerBet(const cv::Mat& playerBetImage) const -> int32_t         = 0;
            [[nodiscard]] virtual auto readPlayerBetInBB(const cv::Mat& playerBetInBBImage) const -> double  = 0;
            [[nodiscard]] virtual auto readPlayerName(const cv::Mat& playerNameImage) const -> std::string   = 0;
            [[nodiscard]] virtual auto readPot(const cv::Mat& potImage) const -> int32_t                     = 0;
            [[nodiscard]] virtual auto readPotInBB(const cv::Mat& potInBBImage) const -> double              = 0;
            [[nodiscard]] virtual auto readPrizePool(const cv::Mat& prizePoolImage) const -> int32_t         = 0;
            [[nodiscard]] virtual auto readSmallBlind(const cv::Mat& blindRangeImage) const -> int32_t       = 0;
            [[nodiscard]] virtual auto hasFolded(const cv::Mat& handImage) const -> bool                     = 0;
            [[nodiscard]] virtual auto hasButton(const cv::Mat& buttonImage) const -> bool                   = 0;

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
