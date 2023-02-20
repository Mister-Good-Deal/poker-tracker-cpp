#pragma once

namespace Scraper {
    class ScraperInterface {
        public:
            ScraperInterface()                              = default;
            ScraperInterface(const ScraperInterface& other) = default;
            //            ScraperInterface(ScraperInterface&& other) noexcept { *this = std::move(other); };

            virtual ~ScraperInterface() = default;

            auto operator=(const ScraperInterface& other) -> ScraperInterface& = default;
            auto operator=(ScraperInterface&& other) noexcept -> ScraperInterface&;
    };
}  // namespace Scraper
