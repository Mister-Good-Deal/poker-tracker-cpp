#pragma once

#include <string>
#include <unordered_map>

#include <opencv4/opencv2/opencv.hpp>

#include <Logger.hpp>

#ifdef _WIN32

    #include <windows.h>

#elif __linux__

    #include <X11/Xlib.h>
    #include <X11/Xos.h>
    #include <X11/Xutil.h>

#endif
namespace Scraper {
    struct WindowInfo {
            std::string title;
            uint64_t    id;
#ifdef _WIN32
            HWND ref;

            WindowInfo(std::string_view title, HWND ref) : title(title), id(std::hash<std::string_view>{}(title), ref(ref)) {}
#elif __linux__
            Window ref;

            WindowInfo(std::string_view title, Window ref) : title(title), id(ref), ref(ref) {}
#endif
    };

    class ScraperInterface {
        public:
            using windows_t = std::unordered_map<uint64_t, WindowInfo>;

            ScraperInterface()                              = default;
            ScraperInterface(const ScraperInterface& other) = default;
            ScraperInterface(ScraperInterface&& other) noexcept { *this = std::move(other); };

            virtual ~ScraperInterface() = default;

            auto operator=(const ScraperInterface& other) -> ScraperInterface& = default;
            auto operator=(ScraperInterface&& other) noexcept -> ScraperInterface&;

            auto getActiveWindows() -> windows_t;
            auto getScreenshot(uint64_t windowId) -> cv::Mat;
            auto getWindowElementsView(const cv::Mat& img) -> cv::Mat;

            auto getFirstCardImg(const cv::Mat& img) -> cv::Mat { return img(getFirstCardCoordinate()); };
            auto getSecondCardImg(const cv::Mat& img) -> cv::Mat { return img(getSecondCardCoordinate()); };
            auto getPotImg(const cv::Mat& img) -> cv::Mat { return img(getPotCoordinate()); };
            auto getPrizePoolImg(const cv::Mat& img) -> cv::Mat { return img(getPrizePoolCoordinate()); };
            auto getBoardCard1Img(const cv::Mat& img) -> cv::Mat { return img(getBoardCard1Coordinate()); };
            auto getBoardCard2Img(const cv::Mat& img) -> cv::Mat { return img(getBoardCard2Coordinate()); };
            auto getBoardCard3Img(const cv::Mat& img) -> cv::Mat { return img(getBoardCard3Coordinate()); };
            auto getBoardCard4Img(const cv::Mat& img) -> cv::Mat { return img(getBoardCard4Coordinate()); };
            auto getBoardCard5Img(const cv::Mat& img) -> cv::Mat { return img(getBoardCard5Coordinate()); };
            auto getPlayer1NameImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer1NameCoordinate()); };
            auto getPlayer2NameImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer2NameCoordinate()); };
            auto getPlayer3NameImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer3NameCoordinate()); };
            auto getPlayer1ButtonImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer1ButtonCoordinate()); };
            auto getPlayer2ButtonImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer2ButtonCoordinate()); };
            auto getPlayer3ButtonImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer3ButtonCoordinate()); };
            auto getPlayer1StackImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer1StackCoordinate()); };
            auto getPlayer2StackImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer2StackCoordinate()); };
            auto getPlayer3StackImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer3StackCoordinate()); };
            auto getPlayer1BetImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer1BetCoordinate()); };
            auto getPlayer2BetImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer2BetCoordinate()); };
            auto getPlayer3BetImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer3BetCoordinate()); };
            auto getPlayer2CardsImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer2HandCoordinate()); };
            auto getPlayer3CardsImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer3HandCoordinate()); };

            // All screen elements coordinates from top left corner (0, 0) in top left(x, y) cv::Rect(x, y, width, eight)
            virtual auto getFirstCardCoordinate() -> cv::Rect     = 0;
            virtual auto getSecondCardCoordinate() -> cv::Rect    = 0;
            virtual auto getPotCoordinate() -> cv::Rect           = 0;
            virtual auto getPrizePoolCoordinate() -> cv::Rect     = 0;
            virtual auto getBoardCard1Coordinate() -> cv::Rect    = 0;
            virtual auto getBoardCard2Coordinate() -> cv::Rect    = 0;
            virtual auto getBoardCard3Coordinate() -> cv::Rect    = 0;
            virtual auto getBoardCard4Coordinate() -> cv::Rect    = 0;
            virtual auto getBoardCard5Coordinate() -> cv::Rect    = 0;
            virtual auto getPlayer1NameCoordinate() -> cv::Rect   = 0;
            virtual auto getPlayer2NameCoordinate() -> cv::Rect   = 0;
            virtual auto getPlayer3NameCoordinate() -> cv::Rect   = 0;
            virtual auto getPlayer1ButtonCoordinate() -> cv::Rect = 0;
            virtual auto getPlayer2ButtonCoordinate() -> cv::Rect = 0;
            virtual auto getPlayer3ButtonCoordinate() -> cv::Rect = 0;
            virtual auto getPlayer1StackCoordinate() -> cv::Rect  = 0;
            virtual auto getPlayer2StackCoordinate() -> cv::Rect  = 0;
            virtual auto getPlayer3StackCoordinate() -> cv::Rect  = 0;
            virtual auto getPlayer1BetCoordinate() -> cv::Rect    = 0;
            virtual auto getPlayer2BetCoordinate() -> cv::Rect    = 0;
            virtual auto getPlayer3BetCoordinate() -> cv::Rect    = 0;
            virtual auto getPlayer2HandCoordinate() -> cv::Rect   = 0;
            virtual auto getPlayer3HandCoordinate() -> cv::Rect   = 0;

        private:
            windows_t _activeWindows;

            auto _parseActiveWindows() -> void;
    };
}  // namespace Scraper
