#include "WinamaxScraper.hpp"

namespace Scraper {
    using Logger = Logger::Quill;

    auto WinamaxScraper::operator=(WinamaxScraper&& other) noexcept -> WinamaxScraper& {
        if (this != &other)
        {
            // @todo, prefer using var = std::move(...);
        }

        return *this;
    }

    auto WinamaxScraper::_parseActiveWindows() -> void {
        LOG_INFO(Logger::getLogger(), "_parseActiveWindows");
#ifdef _WIN32
        HWND hwnd = nullptr;

        while ((hwnd = FindWindowEx(nullptr, hwnd, nullptr, nullptr)) != nullptr)
        {
            char title[1024];
            GetWindowText(hwnd, title, sizeof(title));
            if (strlen(title) > 0) {_activeWindows[title] = hwnd); }
        }

#elif __linux__
        Display*          display = XOpenDisplay(nullptr);
        Window            parent, root, *children = nullptr;
        XWindowAttributes attributes;
        uint32_t          childrenNumber = 0;

        if (XQueryTree(display, DefaultRootWindow(display), &root, &parent, &children, &childrenNumber))
        {
            for (uint32_t i = 0; i < childrenNumber; ++i)
            {
                if (XGetWindowAttributes(display, children[i], &attributes) && attributes.map_state == IsViewable)
                {
                    char* title = nullptr;

                    if (XFetchName(display, children[i], &title))
                    {
                        _activeWindows[title] = children[i];
                        LOG_INFO(Logger::getLogger(), "active window's name: {}", title);
                        //                        LOG_DEBUG(Logger::getLogger(), "active window's name: {}", title);
                        XFree(title);
                    }
                }
            }

            XFree(children);
        }

        XCloseDisplay(display);
#endif
    }

    auto WinamaxScraper::getScreenshot(const std::string& windowName) -> cv::Mat {
        _parseActiveWindows();

        auto window = _activeWindows[windowName];

#ifdef _WIN32
        RECT rc;
        GetClientRect(window, &rc);

        HDC hdcScreen = GetDC(nullptr);
        HDC hdcWindow = GetDC(screenshot);
        HDC hdcMemDC  = CreateCompatibleDC(hdcScreen);

        int32_t width  = rc.right - rc.left;
        int32_t height = rc.bottom - rc.top;

        HBITMAP hbmScreen = CreateCompatibleBitmap(hdcScreen, width, height);
        HGDIOBJ oldObj    = SelectObject(hdcMemDC, hbmScreen);

        BitBlt(hdcMemDC, 0, 0, width, height, hdcWindow, 0, 0, SRCCOPY);

        cv::Mat screenshot(height, width, CV_8UC4);

        GetBitmapBits(hbmScreen, mat.total() * mat.elemSize(), mat.data);

        screenshot = cv::imdecode(mat, cv::IMREAD_COLOR);

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
}  // namespace Scraper
