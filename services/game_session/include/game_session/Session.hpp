#pragma once

#include <game_handler/Game.hpp>
#include <ocr/OcrFactory.hpp>
#include <scraper/Model.hpp>
#include <utilities/Image.hpp>

namespace GameSession {
    using GameHandler::Game;
    using GameHandler::Player;
    using GameHandler::Round;
    using GameHandler::RoundAction;
    using OCR::OcrInterface;
    using OCR::Factory::OcrFactory;
    using Scraper::windowSize_t;
    using std::chrono::milliseconds;
    using std::chrono::seconds;
    using Utilities::Image::isSimilar;
    using Utilities::Image::SIMILARITY_THRESHOLD;
    using Utilities::Time::now;
    using Utilities::Time::time_point;

    using sharedConstMat_t = Scraper::Model::sharedConstMat_t;
    using ActionType       = GameHandler::RoundAction::ActionType;
    using F1               = std::function<const cv::Mat(const cv::Mat&)>;  // Scraper::Model functions that take only one argument
    using F2 = std::function<const cv::Mat(const cv::Mat&, int32_t)>;       // Scraper::Model functions that take two arguments

    static const milliseconds TICK_RATE                   = milliseconds(500);
    static const double       ACTION_SIMILARITY_THRESHOLD = 0.01;

    enum GameStages : int32_t { STARTING = 0, GAME_INFO_SETUP, WAITING_NEW_ROUND, ROUND_IN_PROGRESS, ENDED };

    class CannotFindButtonException : public std::exception {
        public:
            explicit CannotFindButtonException() = default;

            [[nodiscard]] auto what() const noexcept -> const char* override { return "Cannot find the button"; }
    };

    template<typename ScrapperFn> struct CheckedImg {
        public:
            explicit CheckedImg(ScrapperFn getterFn, double similarityThreshold = 0.01)
              : _getterFn(std::move(getterFn))
              , _similarityThreshold(similarityThreshold) {}

            [[nodiscard]] auto getImg() const -> const cv::Mat& { return _currentImg; }
            [[nodiscard]] auto getComparisonImg() const -> const cv::Mat& { return _comparisonImg; }
            [[nodiscard]] auto getLastImg() const -> const cv::Mat& { return _lastImg; }

            auto setComparisonImg(const cv::Mat& currentFrame, int32_t playerNum = 0) -> void {
                // ScrapperFn can be a function that takes only one argument F1 or two arguments F2
                if constexpr (std::is_same_v<ScrapperFn, F1>) {
                    _comparisonImg = _getterFn(currentFrame);
                } else if constexpr (std::is_same_v<ScrapperFn, F2>) {
                    _comparisonImg = _getterFn(currentFrame, playerNum);
                }
            }

            auto setCurrentImg(const cv::Mat& currentFrame, int32_t playerNum = 0) -> void {
                std::swap(_currentImg, _lastImg);
                // ScrapperFn can be a function that takes only one argument F1 or two arguments F2
                if constexpr (std::is_same_v<ScrapperFn, F1>) {
                    _currentImg = _getterFn(currentFrame);
                } else if constexpr (std::is_same_v<ScrapperFn, F2>) {
                    _currentImg = _getterFn(currentFrame, playerNum);
                }
            }

            [[nodiscard]] auto isChecked() const -> bool {
                return !_currentImg.empty() && !_lastImg.empty() && isSimilar(_currentImg, _lastImg, _similarityThreshold);
            }

            [[nodiscard]] auto hasChanged(const cv::Mat& currentFrame, int32_t playerNum = 0) const -> bool {
                cv::Mat currentImg;

                if constexpr (std::is_same_v<ScrapperFn, F1>) {
                    currentImg = _getterFn(currentFrame);
                } else if constexpr (std::is_same_v<ScrapperFn, F2>) {
                    currentImg = _getterFn(currentFrame, playerNum);
                }
                return !isSimilar(currentImg, _comparisonImg, _similarityThreshold);
            }

        private:
            ScrapperFn _getterFn;
            cv::Mat    _currentImg;
            cv::Mat    _lastImg;
            cv::Mat    _comparisonImg;
            double     _similarityThreshold;
    };

    /**
     * @brief This class is used to store the images that we need to check if they are the same as the last frame, by player.
     *
     * @tparam ScrapperFnByPlayer
     */
    template<typename ScrapperFnByPlayer> struct CheckedImgN {
        public:
            explicit CheckedImgN(ScrapperFnByPlayer getterFn, double similarityThreshold = SIMILARITY_THRESHOLD)
              : _getterFn(std::move(getterFn))
              , _similarityThreshold(similarityThreshold) {}

            [[nodiscard]] auto getImg(int32_t playerNum) const -> const cv::Mat& {
                switch (playerNum) {
                    case 1: return _player1.getImg();
                    case 2: return _player2.getImg();
                    case 3: return _player3.getImg();
                    default: throw std::invalid_argument("Invalid player number");
                }
            }

            auto setCurrentImg(const cv::Mat& currentFrame, int32_t playerNum) -> void {
                switch (playerNum) {
                    case 1: _player1.setCurrentImg(currentFrame, 1); break;
                    case 2: _player2.setCurrentImg(currentFrame, 2); break;
                    case 3: _player3.setCurrentImg(currentFrame, 3); break;
                    default: throw std::invalid_argument("Invalid player number");
                }
            }

            auto setComparisonImg(const cv::Mat& currentFrame, int32_t playerNum) -> void {
                switch (playerNum) {
                    case 1: _player1.setComparisonImg(currentFrame, 1); break;
                    case 2: _player2.setComparisonImg(currentFrame, 2); break;
                    case 3: _player3.setComparisonImg(currentFrame, 3); break;
                    default: throw std::invalid_argument("Invalid player number");
                }
            }

            [[nodiscard]] auto hasChanged(const cv::Mat& currentFrame, int32_t playerNum) const -> bool {
                switch (playerNum) {
                    case 1: return _player1.hasChanged(currentFrame, 1);
                    case 2: return _player2.hasChanged(currentFrame, 2);
                    case 3: return _player3.hasChanged(currentFrame, 3);
                    default: throw std::invalid_argument("Invalid player number");
                }
            }

            [[nodiscard]] auto isChecked(int32_t playerNum) const -> bool {
                switch (playerNum) {
                    case 1: return _player1.isChecked();
                    case 2: return _player2.isChecked();
                    case 3: return _player3.isChecked();
                    default: throw std::invalid_argument("Invalid player number");
                }
            }

        private:
            ScrapperFnByPlayer             _getterFn;
            double                         _similarityThreshold;
            CheckedImg<ScrapperFnByPlayer> _player1 {_getterFn, _similarityThreshold};
            CheckedImg<ScrapperFnByPlayer> _player2 {_getterFn, _similarityThreshold};
            CheckedImg<ScrapperFnByPlayer> _player3 {_getterFn, _similarityThreshold};
    };

    class Session {
        public:
            Session(std::string_view roomName, uint64_t windowId, windowSize_t windowSize = {0, 0});
            Session(const Session& other) = delete;
            Session(Session&& other) noexcept { *this = std::move(other); };

            virtual ~Session() = default;

            auto operator=(const Session& other) -> Session& = delete;
            auto operator=(Session&& other) noexcept -> Session&;

            auto getGame() -> Game& { return _game; }

            auto run() -> void;

        protected:
            virtual auto _getScreenshot() -> sharedConstMat_t;

        private:
            milliseconds                  _tickRate = TICK_RATE;
            time_point                    _lastTick;
            std::string                   _roomName;
            uint64_t                      _windowId   = 0;
            windowSize_t                  _windowSize = {0, 0};
            Scraper::Model                _scraper    = Scraper::Model(_roomName, _windowSize);
            std::unique_ptr<OcrInterface> _ocr;
            Game                          _game;
            GameStages                    _gameStage = GameStages::STARTING;
            sharedConstMat_t              _currentScreenshot;
            int32_t                       _currentButtonNum  = 0;
            int32_t                       _currentPlayerNum  = 0;
            ActionType                    _currentAction     = ActionType::NONE;
            bool                          _showdownTriggered = false;
            cv::Mat                       _lastWaitingActionImg;
            // All read images that we need to check if they are the same as the last frame
            CheckedImgN<F2> _playerActionImg {[&](const cv::Mat& img, int32_t num) { return _scraper.getPlayerActionImg(img, num); }};
            CheckedImgN<F2> _playerBetImg {[&](const cv::Mat& img, int32_t num) { return _scraper.getPlayerBetImg(img, num); }};
            CheckedImg<F1>  _board1CardImg {[&](const cv::Mat& img) { return _scraper.getBoardCard1Img(img); }};
            CheckedImg<F1>  _board2CardImg {[&](const cv::Mat& img) { return _scraper.getBoardCard2Img(img); }};
            CheckedImg<F1>  _board3CardImg {[&](const cv::Mat& img) { return _scraper.getBoardCard3Img(img); }};
            CheckedImg<F1>  _board4CardImg {[&](const cv::Mat& img) { return _scraper.getBoardCard4Img(img); }};
            CheckedImg<F1>  _board5CardImg {[&](const cv::Mat& img) { return _scraper.getBoardCard5Img(img); }};

            auto _determineGameOver() -> void;
            auto _determinePlayerAction(const cv::Mat& screenshot, const cv::Mat& actionImg, int32_t playerNum) -> void;
            auto _determinePlayerActionFallback(const cv::Mat& screenshot, int32_t playerNum) -> ActionType;
            auto _getButtonPosition(const cv::Mat& screenshot) -> int32_t;
            auto _getFlop(const cv::Mat& screenshot, bool checkHasChanged = false) -> void;
            auto _getTurn(const cv::Mat& screenshot, bool checkHasChanged = false) -> void;
            auto _getRiver(const cv::Mat& screenshot, bool checkHasChanged = false) -> void;
            auto _getStreetCards(const cv::Mat& screenshot, const Round& round) -> void;
            auto _harvestGameInfo(const cv::Mat& screenshot) -> void;
            auto _isNextActionTriggered(const cv::Mat& screenshot) -> bool;
            auto _trackCurrentRound(const cv::Mat& screenshot) -> void;
            auto _waitGameStart(const cv::Mat& screenshot) -> void;
            auto _waitNewRound(const cv::Mat& screenshot) -> void;
            auto _waitShowdown(const cv::Mat& screenshot) -> void;
            auto _setShowdownComparisonCards(const cv::Mat& screenshot) -> void;
            auto _endRound() -> void;
    };
}  // namespace GameSession