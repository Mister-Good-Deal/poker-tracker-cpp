#include "WinamaxScraper.hpp"

namespace Scraper {
    auto WinamaxScraper::operator=(WinamaxScraper&& other) noexcept -> WinamaxScraper& {
        if (this != &other)
        {
            // @todo, prefer using var = std::move(...);
        }

        return *this;
    }
}  // namespace Scraper
