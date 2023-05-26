#pragma once

#include <ScraperInterface.hpp>

namespace Scraper::Factory {

    class invalid_scraper : public std::runtime_error {
        public:
            explicit invalid_scraper(const std::string& arg) : runtime_error(arg){};
    };

    class ScraperFactory {
        public:
            ScraperFactory() = delete;

            static auto create(const std::string& scraperName) -> std::unique_ptr<ScraperInterface>;
    };
}  // namespace Scraper::Factory