#include "ScraperFactory.hpp"

namespace Scraper::Factory {
    auto ScraperFactory::create(const std::string& scraperName) -> std::unique_ptr<ScraperInterface> {
        if (!SCRAPERS_NAMES.contains(scraperName)) { throw invalid_scraper("Invalid scraper name (" + scraperName + ")"); }

        std::unique_ptr<ScraperInterface> scraper;

        if (scraperName == "Winamax") { scraper = std::make_unique<WinamaxScraper>(); }

        return scraper;
    }
}  // namespace Scraper::Factory