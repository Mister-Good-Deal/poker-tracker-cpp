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

    enum GameStages : int8_t { STARTING = 0, IN_PROGRESS, ENDED };

    enum GameEvent : int8_t { PLAYER_ACTION = 0, GAME_ACTION, NONE };

    class PotNotInitializedException : public std::runtime_error {
        public:
            explicit PotNotInitializedException(const std::string& arg) : runtime_error(arg) {}
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
            auto _determinePlayerAction(const cv::Mat& screenshot, const Player& player) -> void;
            auto _isGameOver() -> bool;

        private:
            milliseconds                  _tickRate = milliseconds(TICK_RATE);
            std::string                   _roomName;
            uint64_t                      _windowId = 0;
            Scraper::Model                _scraper  = Scraper::Model(_roomName, {0, 0});
            std::unique_ptr<OcrInterface> _ocr;
            Game                          _game;
            GameStages                    _gameStage = GameStages::STARTING;
            cv::Mat                       _currentScreenshot;
            const Player*                 _currentPlaying = nullptr;

            auto _processPlayerAction(const RoundAction& action) -> void;
            auto _determineGameEvent() -> GameEvent;
    };
}  // namespace GameSession