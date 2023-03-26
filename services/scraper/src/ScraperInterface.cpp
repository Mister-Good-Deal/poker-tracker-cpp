#include "ScraperInterface.hpp"

#include <fmt/core.h>
#include <ranges>

namespace Scraper {
    using std::ranges::ref_view;
    using std::views::keys;

    using Logger = Logger::Quill;

    auto ScraperInterface::operator=(ScraperInterface&& other) noexcept -> ScraperInterface& {
        if (this != &other) { _activeWindows = std::move(other._activeWindows); }

        return *this;
    }

    auto ScraperInterface::_parseActiveWindows() -> void {
#ifdef _WIN32
        LOG_DEBUG(Logger::getLogger(), "Enter _parseActiveWindows");
        HWND hwnd = nullptr;
        LOG_DEBUG(Logger::getLogger(), "Call FindWindowEx");
        while ((hwnd = FindWindowEx(nullptr, hwnd, nullptr, nullptr)) != nullptr)
        {
            LOG_DEBUG(Logger::getLogger(), "Call GetWindowTextLength");
            auto titleLength = GetWindowTextLength(hwnd);
            LOG_DEBUG(Logger::getLogger(), "Call VirtualAlloc");
            auto title = static_cast<PSTR>(VirtualAlloc(nullptr, titleLength + 1, MEM_COMMIT, PAGE_READWRITE));
            LOG_DEBUG(Logger::getLogger(), "Call GetWindowText");
            auto titleSize = GetWindowText(hwnd, title, titleLength);

            if (titleSize == 0)
            {
                LOG_DEBUG(Logger::getLogger(), "The windows's title could not be retrieved\n\n{}", GetLastError());
            } else {
                _activeWindows[title] = hwnd;
            }

            VirtualFree(title, 0, MEM_RELEASE);
        }

#elif __linux__
        Display*          display = XOpenDisplay(nullptr);
        Window            parent{}, root{}, *children = nullptr;
        XWindowAttributes attributes;
        uint32_t          childrenNumber = 0;

        if (XQueryTree(display, DefaultRootWindow(display), &root, &parent, &children, &childrenNumber))
        {
            // C++ way to avoid pointer arithmetic use
            auto childrenArray = std::span(children, childrenNumber);

            for (uint32_t i = 0; i < childrenNumber; ++i)
            {
                if (XGetWindowAttributes(display, childrenArray[i], &attributes) && attributes.map_state == IsViewable)
                {
                    char* title = nullptr;

                    if (XFetchName(display, childrenArray[i], &title))
                    {
                        _activeWindows[title] = childrenArray[i];
                        XFree(title);
                    }
                }
            }

            XFree(children);
        }

        XCloseDisplay(display);
#endif
    }

    auto ScraperInterface::getScreenshot(std::string_view windowName) -> cv::Mat {
        _parseActiveWindows();

        if (!_activeWindows.contains(windowName.data()))
        { throw std::invalid_argument(fmt::format("The window's name {} is not found", windowName)); }

        auto window = _activeWindows[windowName.data()];

#ifdef _WIN32
        RECT rc;

        GetClientRect(window, &rc);

        HDC hdcScreen = GetDC(nullptr);
        HDC hdcWindow = GetDC(window);

        int width  = rc.right - rc.left;
        int height = rc.bottom - rc.top;

        HDC     hdcMemDC  = CreateCompatibleDC(hdcScreen);
        HBITMAP hbmScreen = CreateCompatibleBitmap(hdcScreen, width, height);
        HGDIOBJ oldObj    = SelectObject(hdcMemDC, hbmScreen);

        BitBlt(hdcMemDC, 0, 0, width, height, hdcWindow, 0, 0, SRCCOPY);

        cv::Mat screenshot(height, width, CV_8UC4);

        GetBitmapBits(hbmScreen, static_cast<LONG>(screenshot.total() * screenshot.elemSize()), screenshot.data);

        screenshot = cv::imdecode(screenshot, cv::IMREAD_COLOR);

        SelectObject(hdcMemDC, oldObj);
        DeleteDC(hdcMemDC);
        ReleaseDC(nullptr, hdcScreen);
        ReleaseDC(window, hdcWindow);
        DeleteObject(hbmScreen);

#elif __linux__
        Display*          display = XOpenDisplay(nullptr);
        XWindowAttributes attributes;
        XGetWindowAttributes(display, window, &attributes);
        XImage* img = XGetImage(display, window, 0, 0, attributes.width, attributes.height, AllPlanes, ZPixmap);

        cv::Mat screenshot(attributes.height, attributes.width, CV_8UC4, img->data);

        cv::cvtColor(screenshot, screenshot, cv::COLOR_BGRA2BGR);

        XDestroyImage(img);
        XCloseDisplay(display);
#endif

        return screenshot;
    }

    auto ScraperInterface::getWindowsName() -> std::vector<std::string_view> {
        _parseActiveWindows();

        std::vector<std::string_view> windowsName{};

        for (const auto& windowName : _activeWindows | keys)
        { windowsName.emplace_back(windowName); }

        return windowsName;
    }
}  // namespace Scraper
