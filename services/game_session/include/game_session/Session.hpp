#pragma once

#include <game_handler/Game.hpp>
#include <ocr/OcrFactory.hpp>
#include <scraper/Model.hpp>

namespace GameSession {
    using GameHandler::Game;
    using GameHandler::Player;
    using GameHandler::RoundAction;
    using OCR::OcrInterface;
    using OCR::Factory::OcrFactory;
    using std::chrono::milliseconds;
    using std::chrono::seconds;

    static const milliseconds TICK_RATE = milliseconds(500);

    enum GameStages : int32_t { STARTING = 0, GAME_INFO_SETUP, IN_PROGRESS, ENDED };

    class InvalidActionException : public std::runtime_error {
        public:
            explicit InvalidActionException(const std::string& arg) : runtime_error(arg) {}
    };

    class CannotFindButtonException : public std::runtime_error {
        public:
            explicit CannotFindButtonException(const std::string& arg) : runtime_error(arg) {}
    };

    class Session {
        public:
            Session(std::string_view roomName, uint64_t windowId);
            Session(const Session& other) = delete;
            Session(Session&& other) noexcept { *this = std::move(other); };

            virtual ~Session() = default;

            auto operator=(const Session& other) -> Session& = delete;
            auto operator=(Session&& other) noexcept -> Session&;

            auto getGame() -> Game& { return _game; }

            auto run() -> void;

        protected:
            virtual auto _getScreenshot() -> cv::Mat;

        private:
            milliseconds                  _tickRate = TICK_RATE;
            std::string                   _roomName;
            uint64_t                      _windowId = 0;
            Scraper::Model                _scraper  = Scraper::Model(_roomName, {0, 0});
            std::unique_ptr<OcrInterface> _ocr;
            Game                          _game;
            GameStages                    _gameStage = GameStages::STARTING;
            cv::Mat                       _currentScreenshot;
            uint32_t                      _currentPlayerPlayingNum = 0;
            cv::Mat                       _lastWaitingActionImg;

            auto _assignButton(const cv::Mat& screenshot) -> void;
            auto _determineGameOver() -> void;
            auto _determinePlayerAction(const cv::Mat& screenshot, uint32_t playerNum) -> void;
            auto _getButtonPosition(const cv::Mat& screenshot) -> int32_t;
            auto _harvestGameInfo(const cv::Mat& screenshot) -> void;
            auto _isNextActionTriggered(const cv::Mat& screenshot) -> bool;
            auto _startRound(const cv::Mat& screenshot) -> void;
            auto _trackCurrentRound(const cv::Mat& screenshot) -> void;
            auto _waitGameStart() -> void;
    };
}  // namespace GameSession