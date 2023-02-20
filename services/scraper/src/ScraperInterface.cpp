#include "ScraperInterface.hpp"

namespace Scraper {
    auto ScraperInterface::operator=(ScraperInterface&& other) noexcept -> ScraperInterface& {
        if (this != &other)
        {
            // @todo, prefer using var = std::move(...);
        }

        return *this;
    }
}  // namespace Scraper
