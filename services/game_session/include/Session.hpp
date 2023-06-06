#pragma once

#include <Game.hpp>
#include <OcrFactory.hpp>
#include <ScraperFactory.hpp>

namespace GameSession {
    using GameHandler::Game;
    using GameHandler::RoundAction;
    using OCR::OcrInterface;
    using OCR::Factory::OcrFactory;
    using Scraper::ScraperInterface;
    using Scraper::Factory::ScraperFactory;
    using std::chrono::milliseconds;

    static const uint32_t TICK_RATE = 500;

    enum GamePhases : int8_t { STARTING = 0, IN_PROGRESS, ENDED };

    enum GameEvent : int8_t { PLAYER_ACTION = 0, GAME_ACTION, NONE };

    class Session {
        public:
            Session(std::string_view roomName, uint64_t windowId);
            Session(const Session& other) = delete;
            Session(Session&& other) noexcept { *this = std::move(other); };

            virtual ~Session() = default;

            auto operator=(const Session& other) -> Session& = delete;
            auto operator=(Session&& other) noexcept -> Session&;

            auto run() -> void;

        private:
            milliseconds                      _tickRate = milliseconds(TICK_RATE);
            std::string                       _roomName;
            uint64_t                          _windowId = 0;
            std::unique_ptr<ScraperInterface> _scraper;
            std::unique_ptr<OcrInterface>     _ocr;
            Game                              _game;
            GamePhases                        _gamePhase = GamePhases::STARTING;
            cv::Mat                           _lastScreenshot;
            cv::Mat                           _currentScreenshot;

            auto _evaluateGamePhase() -> void;
            auto _evaluatePlayerAction() -> RoundAction;
            auto _processPlayerAction(const RoundAction& action) -> void;
            auto _determineGameEvent() -> GameEvent;
            auto _harvestGameInfo() -> void;
    };
}  // namespace GameSession