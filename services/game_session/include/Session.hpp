#pragma once

#include <Game.hpp>
#include <ScraperFactory.hpp>
#include <ScraperInterface.hpp>

namespace GameSession {
    using GameHandler::Game;
    using Scraper::ScraperInterface;
    using Scraper::Factory::ScraperFactory;
    using std::chrono::milliseconds;

    static const uint32_t TICK_RATE = 500;

    class Session {
        public:
            Session(std::string_view roomName, uint64_t windowId);
            Session(const Session& other) = delete;
            Session(Session&& other) noexcept { *this = std::move(other); };

            virtual ~Session() = default;

            auto operator=(const Session& other) -> Session& = delete;
            auto operator=(Session&& other) noexcept -> Session&;

            auto newGameEvent() -> void;

        private:
            milliseconds                      _tickRate = milliseconds(TICK_RATE);
            std::string                       _roomName;
            uint64_t                          _windowId = 0;
            std::unique_ptr<ScraperInterface> _scraper;
            Game                              _game;
    };
}  // namespace GameSession