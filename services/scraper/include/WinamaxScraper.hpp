#pragma once

#include <map>
#include <string>

#include <opencv4/opencv2/opencv.hpp>

#include "Logger.hpp"

#ifdef _WIN32

#include <tlhelp32.h>
#include <windows.h>

#elif __linux__

#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/Xutil.h>

#endif

namespace Scraper {
    class WinamaxScraper {
        public:
#ifdef _WIN32
            using windows_t = std::map<std::string, HWND>;
#elif __linux__
            using windows_t = std::map<std::string, Window>;
#endif

            WinamaxScraper()                            = default;
            WinamaxScraper(const WinamaxScraper& other) = default;
            WinamaxScraper(WinamaxScraper&& other) noexcept { *this = std::move(other); };

            virtual ~WinamaxScraper() = default;

            auto operator=(const WinamaxScraper& other) -> WinamaxScraper& = default;
            auto operator=(WinamaxScraper&& other) noexcept -> WinamaxScraper&;

            auto getScreenshot(const std::string& windowName) -> cv::Mat;

        private:
            windows_t _activeWindows;

            auto _parseActiveWindows() -> void;
    };
}  // namespace Scraper
