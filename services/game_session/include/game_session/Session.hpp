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

    static const uint32_t TICK_RATE = 500;

    enum GameStages : int32_t { STARTING = 0, IN_PROGRESS, ENDED };

    class PotNotInitializedException : public std::runtime_error {
        public:
            explicit PotNotInitializedException(const std::string& arg) : runtime_error(arg) {}
    };

    class WrongCurrentPlayingPlayerException : public std::runtime_error {
        public:
            explicit WrongCurrentPlayingPlayerException(const std::string& arg) : runtime_error(arg) {}
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
            auto _harvestGameInfo(const cv::Mat& screenshot) -> void;
            auto _initCurrentRound(const cv::Mat& screenshot) -> void;
            auto _trackCurrentRound(const cv::Mat& screenshot) -> void;
            auto _determinePlayerAction(const cv::Mat& screenshot, Player& player, uint32_t playerNum) -> void;
            auto _isNextActionTriggered(const cv::Mat& screenshot) -> bool;
            auto _determineGameOver() -> void;

        private:
            milliseconds                  _tickRate = milliseconds(TICK_RATE);
            std::string                   _roomName;
            uint64_t                      _windowId = 0;
            Scraper::Model                _scraper  = Scraper::Model(_roomName, {0, 0});
            std::unique_ptr<OcrInterface> _ocr;
            Game                          _game;
            GameStages                    _gameStage = GameStages::STARTING;
            cv::Mat                       _currentScreenshot;
            Player*                       _currentPlaying    = nullptr;
            uint32_t                      _currentPlayingNum = 0;
            cv::Mat                       _lastWaitingActionImg;

            auto _assignButton(const cv::Mat& screenshot) -> void;
    };
}  // namespace GameSession