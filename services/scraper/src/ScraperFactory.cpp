#include "ScraperFactory.hpp"

#include <WinamaxScraper.hpp>

namespace Scraper::Factory {
    auto ScraperFactory::create(const std::string& scraperName) -> std::unique_ptr<ScraperInterface> {
        if (scraperName == "Winamax") { return std::make_unique<WinamaxScraper>(); }

        throw invalid_scraper("Invalid scraper name: " + scraperName);
    }
}  // namespace Scraper::Factory