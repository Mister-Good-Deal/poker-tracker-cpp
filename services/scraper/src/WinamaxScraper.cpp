#include "WinamaxScraper.hpp"

namespace Scraper {
    auto WinamaxScraper::operator=(WinamaxScraper&& other) noexcept -> WinamaxScraper& {
        if (this != &other)
        {
            // @todo, prefer using var = std::move(...);
        }

        return *this;
    }
    auto WinamaxScraper::getActiveWindows() -> std::vector<window_t> {
        std::vector<window_t> activeWindows;
        int32_t               windowsNumber = SDL_GetNumVideoDisplays();

        for (int32_t i = 0; i < windowsNumber; ++i)
        { activeWindows.emplace_back(i, SDL_GetWindowTitle(SDL_GetWindowFromID(i))); }

        return activeWindows;
    }
}  // namespace Scraper
