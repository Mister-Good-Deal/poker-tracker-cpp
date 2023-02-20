#pragma once

#include <string>
#include <tuple>
#include <vector>

#include <SDL2/SDL.h>

namespace Scraper {
    class WinamaxScraper {
        public:
            using window_t = std::tuple<int32_t, std::string_view>;

            WinamaxScraper()                            = default;
            WinamaxScraper(const WinamaxScraper& other) = default;
            WinamaxScraper(WinamaxScraper&& other) noexcept { *this = std::move(other); };

            virtual ~WinamaxScraper() = default;

            auto operator=(const WinamaxScraper& other) -> WinamaxScraper& = default;
            auto operator=(WinamaxScraper&& other) noexcept -> WinamaxScraper&;

            auto getActiveWindows() -> std::vector<window_t>;
    };
}  // namespace Scraper
