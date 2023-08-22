#include "scraper/Model.hpp"

#include <functional>

#include <fmt/core.h>
#include <ranges>

namespace Scraper {
    using std::ranges::ref_view;
    using std::views::keys;

    using Logger = Logger::Quill;

    Model::Model(std::string_view roomName, windowSize_t windowSize) : _windowSize(windowSize), _roomName(roomName) {
        _loadDefaultModel();
    }

    auto Model::operator=(Model&& other) noexcept -> Model& {
        if (this != &other) {
            _activeWindows = std::move(other._activeWindows);
            _roomName      = std::move(other._roomName);
            _windowSize    = other._windowSize;

            // All elements coordinates
            _firstCardCoord      = other._firstCardCoord;
            _secondCardCoord     = other._secondCardCoord;
            _averageStackCoord   = other._averageStackCoord;
            _blindAmountCoord    = other._blindAmountCoord;
            _blindLevelCoord     = other._blindLevelCoord;
            _blindLevelTimeCoord = other._blindLevelTimeCoord;
            _boardCard1Coord     = other._boardCard1Coord;
            _boardCard2Coord     = other._boardCard2Coord;
            _boardCard3Coord     = other._boardCard3Coord;
            _boardCard4Coord     = other._boardCard4Coord;
            _boardCard5Coord     = other._boardCard5Coord;
            _player1ActionCoord  = other._player1ActionCoord;
            _player1BetCoord     = other._player1BetCoord;
            _player1ButtonCoord  = other._player1ButtonCoord;
            _player1NameCoord    = other._player1NameCoord;
            _player1StackCoord   = other._player1StackCoord;
            _player2ActionCoord  = other._player2ActionCoord;
            _player2BetCoord     = other._player2BetCoord;
            _player2ButtonCoord  = other._player2ButtonCoord;
            _player2HandCoord    = other._player2HandCoord;
            _player2NameCoord    = other._player2NameCoord;
            _player2StackCoord   = other._player2StackCoord;
            _player3ActionCoord  = other._player3ActionCoord;
            _player3BetCoord     = other._player3BetCoord;
            _player3ButtonCoord  = other._player3ButtonCoord;
            _player3HandCoord    = other._player3HandCoord;
            _player3NameCoord    = other._player3NameCoord;
            _player3StackCoord   = other._player3StackCoord;
            _potCoord            = other._potCoord;
            _prizePoolCoord      = other._prizePoolCoord;
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

    auto Model::getScreenshot(uint64_t windowId) -> sharedConstMat_t {
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

        auto screenshot = std::make_shared<cv::Mat>(height, width, CV_8UC4);

        GetBitmapBits(hbmScreen, static_cast<LONG>(screenshot->total() * screenshot->elemSize()), screenshot->data);

        *screenshot = cv::imdecode(*screenshot, cv::IMREAD_COLOR);

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

        auto screenshot = std::make_shared<cv::Mat>(attributes.height, attributes.width, CV_8UC4, img->data);

        cv::cvtColor(*screenshot, *screenshot, cv::COLOR_BGRA2BGR);

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

    auto Model::getWindowElementsView(const cv::Mat& img) const -> const cv::Mat {
        cv::Mat               elementsView = img.clone();
        std::vector<cv::Rect> elements     = {
            getAverageStackCoord(),  getBlindAmountCoord(),   getBlindLevelCoord(),   getBlindLevelTimeCoord(), getBoardCard1Coord(),
            getBoardCard2Coord(),    getBoardCard3Coord(),    getBoardCard4Coord(),   getBoardCard5Coord(),     getFirstCardCoord(),
            getSecondCardCoord(),    getPlayer1ActionCoord(), getPlayer1BetCoord(),   getPlayer1ButtonCoord(),  getPlayer1NameCoord(),
            getPlayer1StackCoord(),  getPlayer2ActionCoord(), getPlayer2BetCoord(),   getPlayer2ButtonCoord(),  getPlayer2CardsCoord(),
            getPlayer2HandCoord(),   getPlayer2NameCoord(),   getPlayer2StackCoord(), getPlayer3ActionCoord(),  getPlayer3BetCoord(),
            getPlayer3ButtonCoord(), getPlayer3CardsCoord(),  getPlayer3HandCoord(),  getPlayer3NameCoord(),    getPlayer3StackCoord(),
            getPotCoord(),           getPrizePoolCoord()};

        for (const auto& element : elements) { cv::rectangle(elementsView, element, cv::Scalar(0, 255, 0), 2); }

        return elementsView;
    }

    auto Model::getPlayerActionCoord(int32_t playerNum) const -> const cv::Rect& {
        switch (playerNum) {
            case 1: return _player1ActionCoord;
            case 2: return _player2ActionCoord;
            case 3: return _player3ActionCoord;
            default: throw std::invalid_argument("Invalid player number");
        }
    }

    auto Model::getPlayerBetCoord(int32_t playerNum) const -> const cv::Rect& {
        switch (playerNum) {
            case 1: return _player1BetCoord;
            case 2: return _player2BetCoord;
            case 3: return _player3BetCoord;
            default: throw std::invalid_argument("Invalid player number");
        }
    }

    auto Model::getPlayerButtonCoord(int32_t playerNum) const -> const cv::Rect& {
        switch (playerNum) {
            case 1: return _player1ButtonCoord;
            case 2: return _player2ButtonCoord;
            case 3: return _player3ButtonCoord;
            default: throw std::invalid_argument("Invalid player number");
        }
    }

    auto Model::getPlayerCardsCoord(int32_t playerNum) const -> const cv::Rect& {
        switch (playerNum) {
            case 2: return _player2CardsCoord;
            case 3: return _player3CardsCoord;
            default: throw std::invalid_argument("Invalid player number");
        }
    }

    auto Model::getPlayerHandCoord(int32_t playerNum) const -> const cv::Rect& {
        switch (playerNum) {
            case 2: return _player2HandCoord;
            case 3: return _player3HandCoord;
            default: throw std::invalid_argument("Invalid player number");
        }
    }

    auto Model::getPlayerStackCoord(int32_t playerNum) const -> const cv::Rect& {
        switch (playerNum) {
            case 1: return _player1StackCoord;
            case 2: return _player2StackCoord;
            case 3: return _player3StackCoord;
            default: throw std::invalid_argument("Invalid player number");
        }
    }

    auto Model::toJson() const -> json {
        return {{"roomName", _roomName},
                {"windowSize", {{"width", _windowSize.width}, {"height", _windowSize.height}}},
                {"elementsBoxes",
                 {{"averageStack", _rectToJson(getAverageStackCoord())},
                  {"blindAmount", _rectToJson(getBlindAmountCoord())},
                  {"blindLevel", _rectToJson(getBlindLevelCoord())},
                  {"blindLevelTime", _rectToJson(getBlindLevelTimeCoord())},
                  {"boardCard1", _rectToJson(getBoardCard1Coord())},
                  {"boardCard2", _rectToJson(getBoardCard2Coord())},
                  {"boardCard3", _rectToJson(getBoardCard3Coord())},
                  {"boardCard4", _rectToJson(getBoardCard4Coord())},
                  {"boardCard5", _rectToJson(getBoardCard5Coord())},
                  {"firstCard", _rectToJson(getFirstCardCoord())},
                  {"secondCard", _rectToJson(getSecondCardCoord())},
                  {"player1Action", _rectToJson(getPlayer1ActionCoord())},
                  {"player1Bet", _rectToJson(getPlayer1BetCoord())},
                  {"player1Button", _rectToJson(getPlayer1ButtonCoord())},
                  {"player1Name", _rectToJson(getPlayer1NameCoord())},
                  {"player1Stack", _rectToJson(getPlayer1StackCoord())},
                  {"player2Action", _rectToJson(getPlayer2ActionCoord())},
                  {"player2Bet", _rectToJson(getPlayer2BetCoord())},
                  {"player2Button", _rectToJson(getPlayer2ButtonCoord())},
                  {"player2Cards", _rectToJson(getPlayer2CardsCoord())},
                  {"player2Hand", _rectToJson(getPlayer2HandCoord())},
                  {"player2Name", _rectToJson(getPlayer2NameCoord())},
                  {"player2Stack", _rectToJson(getPlayer2StackCoord())},
                  {"player3Action", _rectToJson(getPlayer3ActionCoord())},
                  {"player3Bet", _rectToJson(getPlayer3BetCoord())},
                  {"player3Button", _rectToJson(getPlayer3ButtonCoord())},
                  {"player3Cards", _rectToJson(getPlayer3CardsCoord())},
                  {"player3Hand", _rectToJson(getPlayer3HandCoord())},
                  {"player3Name", _rectToJson(getPlayer3NameCoord())},
                  {"player3Stack", _rectToJson(getPlayer3StackCoord())},
                  {"pot", _rectToJson(getPotCoord())},
                  {"prizePool", _rectToJson(getPrizePoolCoord())}}}};
    }

    auto Model::loadFromJson(const json& json) -> void {
        _roomName            = json.at("roomName");
        _averageStackCoord   = _jsonToRect(json.at("elementsBoxes").at("averageStack"));
        _blindAmountCoord    = _jsonToRect(json.at("elementsBoxes").at("blindAmount"));
        _blindLevelCoord     = _jsonToRect(json.at("elementsBoxes").at("blindLevel"));
        _blindLevelTimeCoord = _jsonToRect(json.at("elementsBoxes").at("blindLevelTime"));
        _boardCard1Coord     = _jsonToRect(json.at("elementsBoxes").at("boardCard1"));
        _boardCard2Coord     = _jsonToRect(json.at("elementsBoxes").at("boardCard2"));
        _boardCard3Coord     = _jsonToRect(json.at("elementsBoxes").at("boardCard3"));
        _boardCard4Coord     = _jsonToRect(json.at("elementsBoxes").at("boardCard4"));
        _boardCard5Coord     = _jsonToRect(json.at("elementsBoxes").at("boardCard5"));
        _firstCardCoord      = _jsonToRect(json.at("elementsBoxes").at("firstCard"));
        _secondCardCoord     = _jsonToRect(json.at("elementsBoxes").at("secondCard"));
        _player1ActionCoord  = _jsonToRect(json.at("elementsBoxes").at("player1Action"));
        _player1BetCoord     = _jsonToRect(json.at("elementsBoxes").at("player1Bet"));
        _player1ButtonCoord  = _jsonToRect(json.at("elementsBoxes").at("player1Button"));
        _player1NameCoord    = _jsonToRect(json.at("elementsBoxes").at("player1Name"));
        _player1StackCoord   = _jsonToRect(json.at("elementsBoxes").at("player1Stack"));
        _player2ActionCoord  = _jsonToRect(json.at("elementsBoxes").at("player2Action"));
        _player2BetCoord     = _jsonToRect(json.at("elementsBoxes").at("player2Bet"));
        _player2ButtonCoord  = _jsonToRect(json.at("elementsBoxes").at("player2Button"));
        _player2CardsCoord   = _jsonToRect(json.at("elementsBoxes").at("player2Cards"));
        _player2HandCoord    = _jsonToRect(json.at("elementsBoxes").at("player2Hand"));
        _player2NameCoord    = _jsonToRect(json.at("elementsBoxes").at("player2Name"));
        _player2StackCoord   = _jsonToRect(json.at("elementsBoxes").at("player2Stack"));
        _player3ActionCoord  = _jsonToRect(json.at("elementsBoxes").at("player3Action"));
        _player3BetCoord     = _jsonToRect(json.at("elementsBoxes").at("player3Bet"));
        _player3ButtonCoord  = _jsonToRect(json.at("elementsBoxes").at("player3Button"));
        _player3CardsCoord   = _jsonToRect(json.at("elementsBoxes").at("player3Cards"));
        _player3HandCoord    = _jsonToRect(json.at("elementsBoxes").at("player3Hand"));
        _player3NameCoord    = _jsonToRect(json.at("elementsBoxes").at("player3Name"));
        _player3StackCoord   = _jsonToRect(json.at("elementsBoxes").at("player3Stack"));
        _potCoord            = _jsonToRect(json.at("elementsBoxes").at("pot"));
        _prizePoolCoord      = _jsonToRect(json.at("elementsBoxes").at("prizePool"));
        _windowSize          = {json.at("windowSize").at("width"), json.at("windowSize").at("height")};
    }

    auto Model::_jsonToRect(const json& json) const -> cv::Rect {
        return {json.at("topLeft").at("x"), json.at("topLeft").at("y"), json.at("width"), json.at("height")};
    }

    auto Model::_rectToJson(const cv::Rect& rect) const -> json {
        return {{"topLeft", {{"x", rect.x}, {"y", rect.y}}}, {"width", rect.width}, {"height", rect.height}};
    }
    auto Model::_loadDefaultModel() -> void {
        std::filesystem::path modelPath;

        if (_roomName == "Winamax") {
            modelPath = fmt::format(MODEL_PATH_FORMAT, fmt::arg("dir", WINAMAX_DIR), fmt::arg("size", _windowSize));
        }

        std::ifstream file(modelPath);

        if (file.is_open()) {
            loadFromJson(json::parse(file));
        } else {
            throw std::runtime_error(fmt::format("No default model found for room {} of window's size {}", _roomName, _windowSize));
        }
    }
}  // namespace Scraper
