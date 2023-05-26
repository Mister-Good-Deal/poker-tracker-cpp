#include "Session.hpp"

namespace GameSession {
    Session::Session(std::string_view roomName, uint64_t windowId) :
        _roomName(roomName), _windowId(windowId), _scraper(ScraperFactory::create(_roomName)), _game() {}

    auto Session::operator=(Session&& other) noexcept -> Session& {
        if (this != &other)
        {
            _game     = std::move(other._game);
            _scraper  = std::move(other._scraper);
            _tickRate = other._tickRate;
            _windowId = other._windowId;
        }

        return *this;
    }
}  // namespace GameSession