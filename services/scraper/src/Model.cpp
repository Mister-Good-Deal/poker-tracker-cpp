#include "scraper/Model.hpp"

#include <functional>

#include <fmt/core.h>
#include <ranges>

namespace Scraper {
    using std::ranges::ref_view;
    using std::views::keys;

    using Logger = Logger::Quill;

    auto Model::operator=(Model&& other) noexcept -> Model& {
        if (this != &other) {
            _activeWindows = std::move(other._activeWindows);
            _roomName      = std::move(other._roomName);
            _windowSize    = std::move(other._windowSize);

            // All elements coordinates
            _firstCardCoord     = other._firstCardCoord;
            _secondCardCoord    = other._secondCardCoord;
            _potCoord           = other._potCoord;
            _prizePoolCoord     = other._prizePoolCoord;
            _boardCard1Coord    = other._boardCard1Coord;
            _boardCard2Coord    = other._boardCard2Coord;
            _boardCard3Coord    = other._boardCard3Coord;
            _boardCard4Coord    = other._boardCard4Coord;
            _boardCard5Coord    = other._boardCard5Coord;
            _player1NameCoord   = other._player1NameCoord;
            _player2NameCoord   = other._player2NameCoord;
            _player3NameCoord   = other._player3NameCoord;
            _player1ButtonCoord = other._player1ButtonCoord;
            _player2ButtonCoord = other._player2ButtonCoord;
            _player3ButtonCoord = other._player3ButtonCoord;
            _player1StackCoord  = other._player1StackCoord;
            _player2StackCoord  = other._player2StackCoord;
            _player3StackCoord  = other._player3StackCoord;
            _player1BetCoord    = other._player1BetCoord;
            _player2BetCoord    = other._player2BetCoord;
            _player3BetCoord    = other._player3BetCoord;
            _player2HandCoord   = other._player2HandCoord;
            _player3HandCoord   = other._player3HandCoord;
            _blindLevelCoord    = other._blindLevelCoord;
            _blindAmountCoord   = other._blindAmountCoord;
            _gameTimeCoord      = other._gameTimeCoord;
        }

        return *this;
    }

    auto Model::_parseActiveWindows() -> void {
#ifdef _WIN32
        LOG_DEBUG(Logger::getLogger(), "Enter _parseActiveWindows");
        HWND hwnd = nullptr;
        LOG_DEBUG(Logger::getLogger(), "Call FindWindowEx");
        while ((hwnd = FindWindowEx(nullptr, hwnd, nullptr, nullptr)) != nullptr) {
            LOG_DEBUG(Logger::getLogger(), "Call GetWindowTextLength");
            auto titleLength = GetWindowTextLength(hwnd);
            LOG_DEBUG(Logger::getLogger(), "Call VirtualAlloc");
            auto title = static_cast<PSTR>(VirtualAlloc(nullptr, titleLength + 1, MEM_COMMIT, PAGE_READWRITE));
            LOG_DEBUG(Logger::getLogger(), "Call GetWindowText");
            auto titleSize = GetWindowText(hwnd, title, titleLength);

            if (titleSize == 0) {
                LOG_DEBUG(Logger::getLogger(), "The windows's title could not be retrieved\n\n{}", GetLastError());
            } else {
                WindowInfo window(title, hwnd);

                _activeWindows.emplace(window.id, window);
            }

            VirtualFree(title, 0, MEM_RELEASE);
        }

#elif __linux__
        Display*          display = XOpenDisplay(nullptr);
        Window            parent{}, root{}, *children = nullptr;
        XWindowAttributes attributes;
        uint32_t          childrenNumber = 0;

        if (XQueryTree(display, DefaultRootWindow(display), &root, &parent, &children, &childrenNumber)) {
            // C++ way to avoid pointer arithmetic use
            auto childrenArray = std::span(children, childrenNumber);

            for (uint32_t i = 0; i < childrenNumber; ++i) {
                if (XGetWindowAttributes(display, childrenArray[i], &attributes) && attributes.map_state == IsViewable) {
                    char*         title = nullptr;
                    XTextProperty windowName;

                    if (XFetchName(display, childrenArray[i], &title)) {
                        WindowInfo window(title, childrenArray[i]);

                        // Core dump on mutter guard window manager when trying to get the screenshot
                        if (window.title != "mutter guard window") { _activeWindows.emplace(window.id, window); }

                        XFree(title);
                    } else if (XGetWMName(display, childrenArray[i], &windowName)) {
                        WindowInfo window(reinterpret_cast<char*>(windowName.value), childrenArray[i]);

                        _activeWindows.emplace(window.id, window);

                        XFree(windowName.value);
                    } else {
                        WindowInfo window("undefined", childrenArray[i]);

                        _activeWindows.emplace(window.id, window);
                    }
                }
            }

            XFree(children);
        }

        XCloseDisplay(display);
#endif
    }

    auto Model::getScreenshot(uint64_t windowId) -> cv::Mat {
        _parseActiveWindows();

        if (!_activeWindows.contains(windowId)) {
            throw std::invalid_argument(fmt::format("The window's ID {} is not found", windowId));
        }

        auto window = _activeWindows.at(windowId);

#ifdef _WIN32
        RECT rc;

        GetClientRect(window.ref, &rc);

        HDC hdcScreen = GetDC(nullptr);
        HDC hdcWindow = GetDC(window.ref);

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
        ReleaseDC(window.ref, hdcWindow);
        DeleteObject(hbmScreen);

#elif __linux__
        Display*          display = XOpenDisplay(nullptr);
        XWindowAttributes attributes;
        XGetWindowAttributes(display, window.ref, &attributes);
        XImage* img = XGetImage(display, window.ref, 0, 0, attributes.width, attributes.height, AllPlanes, ZPixmap);

        cv::Mat screenshot(attributes.height, attributes.width, CV_8UC4, img->data);

        cv::cvtColor(screenshot, screenshot, cv::COLOR_BGRA2BGR);

        XDestroyImage(img);
        XCloseDisplay(display);
#endif

        return screenshot;
    }

    auto Model::getActiveWindows() -> windows_t {
        _activeWindows.clear();

        _parseActiveWindows();

        return _activeWindows;
    }

    auto Model::getWindowElementsView(const cv::Mat& img) -> cv::Mat {
        cv::Mat               elementsView = img.clone();
        std::vector<cv::Rect> elements     = {
            getFirstCardCoord(),    getSecondCardCoord(),   getPotCoord(),           getPrizePoolCoord(),     getBoardCard1Coord(),
            getBoardCard2Coord(),   getBoardCard3Coord(),   getBoardCard4Coord(),    getBoardCard5Coord(),    getPlayer1NameCoord(),
            getPlayer2NameCoord(),  getPlayer3NameCoord(),  getPlayer1ButtonCoord(), getPlayer2ButtonCoord(), getPlayer3ButtonCoord(),
            getPlayer1StackCoord(), getPlayer2StackCoord(), getPlayer3StackCoord(),  getPlayer1BetCoord(),    getPlayer2BetCoord(),
            getPlayer3BetCoord(),   getPlayer2HandCoord(),  getPlayer3HandCoord(),   getPlayer2ActionCoord(), getPlayer3ActionCoord(),
            getBlindLevelCoord(),   getBlindAmountCoord(),  getGameTimeCoord()};

        for (const auto& element : elements) { cv::rectangle(elementsView, element, cv::Scalar(0, 255, 0), 2); }

        return elementsView;
    }

    auto Model::getPlayerHandCoord(uint32_t playerNum) const -> const cv::Rect& {
        switch (playerNum) {
            case 2: return _player2HandCoord;
            case 3: return _player3HandCoord;
            default: throw std::invalid_argument("Invalid player number");
        }
    }

    auto Model::getPlayerActionCoord(uint32_t playerNum) const -> const cv::Rect& {
        switch (playerNum) {
            case 2: return _player2ActionCoord;
            case 3: return _player3ActionCoord;
            default: throw std::invalid_argument("Invalid player number");
        }
    }

    auto Model::getPlayerBetCoord(uint32_t playerNum) const -> const cv::Rect& {
        switch (playerNum) {
            case 2: return _player2BetCoord;
            case 3: return _player3BetCoord;
            default: throw std::invalid_argument("Invalid player number");
        }
    }

    auto Model::getPlayerStackCoord(uint32_t playerNum) const -> const cv::Rect& {
        switch (playerNum) {
            case 1: return _player1StackCoord;
            case 2: return _player2StackCoord;
            case 3: return _player3StackCoord;
            default: throw std::invalid_argument("Invalid player number");
        }
    }

    auto Model::getPlayerButtonCoord(uint32_t playerNum) const -> const cv::Rect& {
        switch (playerNum) {
            case 1: return _player1ButtonCoord;
            case 2: return _player2ButtonCoord;
            case 3: return _player3ButtonCoord;
            default: throw std::invalid_argument("Invalid player number");
        }
    }

    auto Model::toJson() const -> json {
        return {{"roomName", _roomName},
                {"windowSize", {{"width", _windowSize.first}, {"height", _windowSize.second}}},
                {"elementsBoxes",
                 {{"firstCard", _rectToJson(getFirstCardCoord())},
                  {"secondCard", _rectToJson(getSecondCardCoord())},
                  {"pot", _rectToJson(getPotCoord())},
                  {"prizePool", _rectToJson(getPrizePoolCoord())},
                  {"boardCard1", _rectToJson(getBoardCard1Coord())},
                  {"boardCard2", _rectToJson(getBoardCard2Coord())},
                  {"boardCard3", _rectToJson(getBoardCard3Coord())},
                  {"boardCard4", _rectToJson(getBoardCard4Coord())},
                  {"boardCard5", _rectToJson(getBoardCard5Coord())},
                  {"player1Name", _rectToJson(getPlayer1NameCoord())},
                  {"player2Name", _rectToJson(getPlayer2NameCoord())},
                  {"player3Name", _rectToJson(getPlayer3NameCoord())},
                  {"player1Button", _rectToJson(getPlayer1ButtonCoord())},
                  {"player2Button", _rectToJson(getPlayer2ButtonCoord())},
                  {"player3Button", _rectToJson(getPlayer3ButtonCoord())},
                  {"player1Stack", _rectToJson(getPlayer1StackCoord())},
                  {"player2Stack", _rectToJson(getPlayer2StackCoord())},
                  {"player3Stack", _rectToJson(getPlayer3StackCoord())},
                  {"player1Bet", _rectToJson(getPlayer1BetCoord())},
                  {"player2Bet", _rectToJson(getPlayer2BetCoord())},
                  {"player3Bet", _rectToJson(getPlayer3BetCoord())},
                  {"player2Hand", _rectToJson(getPlayer2HandCoord())},
                  {"player3Hand", _rectToJson(getPlayer3HandCoord())},
                  {"player2Action", _rectToJson(getPlayer2ActionCoord())},
                  {"player3Action", _rectToJson(getPlayer3ActionCoord())},
                  {"blindLevel", _rectToJson(getBlindLevelCoord())},
                  {"blindAmount", _rectToJson(getBlindAmountCoord())},
                  {"gameTime", _rectToJson(getGameTimeCoord())}}}};
    }

    auto Model::loadFromJson(const json& json) -> void {
        _roomName           = json.at("roomName");
        _windowSize         = {json.at("windowSize").at("width"), json.at("windowSize").at("height")};
        _firstCardCoord     = _jsonToRect(json.at("elementsBoxes").at("firstCard"));
        _secondCardCoord    = _jsonToRect(json.at("elementsBoxes").at("secondCard"));
        _potCoord           = _jsonToRect(json.at("elementsBoxes").at("pot"));
        _prizePoolCoord     = _jsonToRect(json.at("elementsBoxes").at("prizePool"));
        _boardCard1Coord    = _jsonToRect(json.at("elementsBoxes").at("boardCard1"));
        _boardCard2Coord    = _jsonToRect(json.at("elementsBoxes").at("boardCard2"));
        _boardCard3Coord    = _jsonToRect(json.at("elementsBoxes").at("boardCard3"));
        _boardCard4Coord    = _jsonToRect(json.at("elementsBoxes").at("boardCard4"));
        _boardCard5Coord    = _jsonToRect(json.at("elementsBoxes").at("boardCard5"));
        _player1NameCoord   = _jsonToRect(json.at("elementsBoxes").at("player1Name"));
        _player2NameCoord   = _jsonToRect(json.at("elementsBoxes").at("player2Name"));
        _player3NameCoord   = _jsonToRect(json.at("elementsBoxes").at("player3Name"));
        _player1ButtonCoord = _jsonToRect(json.at("elementsBoxes").at("player1Button"));
        _player2ButtonCoord = _jsonToRect(json.at("elementsBoxes").at("player2Button"));
        _player3ButtonCoord = _jsonToRect(json.at("elementsBoxes").at("player3Button"));
        _player1StackCoord  = _jsonToRect(json.at("elementsBoxes").at("player1Stack"));
        _player2StackCoord  = _jsonToRect(json.at("elementsBoxes").at("player2Stack"));
        _player3StackCoord  = _jsonToRect(json.at("elementsBoxes").at("player3Stack"));
        _player1BetCoord    = _jsonToRect(json.at("elementsBoxes").at("player1Bet"));
        _player2BetCoord    = _jsonToRect(json.at("elementsBoxes").at("player2Bet"));
        _player3BetCoord    = _jsonToRect(json.at("elementsBoxes").at("player3Bet"));
        _player2HandCoord   = _jsonToRect(json.at("elementsBoxes").at("player2Hand"));
        _player3HandCoord   = _jsonToRect(json.at("elementsBoxes").at("player3Hand"));
        _player2ActionCoord = _jsonToRect(json.at("elementsBoxes").at("player2Action"));
        _player3ActionCoord = _jsonToRect(json.at("elementsBoxes").at("player3Action"));
        _blindLevelCoord    = _jsonToRect(json.at("elementsBoxes").at("blindLevel"));
        _blindAmountCoord   = _jsonToRect(json.at("elementsBoxes").at("blindAmount"));
        _gameTimeCoord      = _jsonToRect(json.at("elementsBoxes").at("gameTime"));
    }

    auto Model::_jsonToRect(const json& json) const -> cv::Rect {
        return {json.at("topLeft").at("x"), json.at("topLeft").at("y"), json.at("width"), json.at("height")};
    }

    auto Model::_rectToJson(const cv::Rect& rect) const -> json {
        return {{"topLeft", {{"x", rect.x}, {"y", rect.y}}}, {"width", rect.width}, {"height", rect.height}};
    }
}  // namespace Scraper
