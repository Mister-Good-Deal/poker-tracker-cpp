#pragma once

#include <unordered_set>

#include <WinamaxScraper.hpp>

namespace Scraper::Factory {

    class invalid_scraper : public std::runtime_error {
        public:
            explicit invalid_scraper(const std::string& arg) : runtime_error(arg){};
    };

    static const std::unordered_set<std::string> SCRAPERS_NAMES = {"Winamax"};

    class ScraperFactory {
        public:
            ScraperFactory() = delete;

            static auto create(const std::string& scraperName) -> std::unique_ptr<ScraperInterface>;
    };
}  // namespace Scraper::Factory