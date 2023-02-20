#include <gtest/gtest.h>

#include "WinamaxScraper.hpp"

using Scraper::WinamaxScraper;

class WinamaxScraperTest : public ::testing::Test {};

TEST(WinamaxScraperTest, listWindows) {
    WinamaxScraper scraper;
    // SDL init
    if (SDL_Init(SDL_INIT_VIDEO) != 0) { std::cerr << "Erreur d'initialisation de la SDL : " << SDL_GetError() << std::endl; }

    for (const auto& [id, name] : scraper.getActiveWindows())
    { std::cout << "[" << std::to_string(id) << "] " << name << std::endl; }

    SDL_Quit();
}
