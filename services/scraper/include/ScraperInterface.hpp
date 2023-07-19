#pragma once

#include <string>
#include <unordered_map>

#include <opencv4/opencv2/opencv.hpp>

#include <nlohmann/json.hpp>

#include <Logger.hpp>

#ifdef _WIN32

    #include <windows.h>

#elif __linux__

    #include <X11/Xlib.h>
    #include <X11/Xos.h>
    #include <X11/Xutil.h>

#endif
namespace Scraper {
    using json = nlohmann::json;

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

            [[nodiscard]] auto toJson() const -> json;

            /**
             *              Players positions on the table
             *
             *          Player 2  __________________  Player 3
             *                   |                  |
             *                   |                  |
             *                   |__________________|
             *                         Player 1
             */
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
            auto getBlindLevelImg(const cv::Mat& img) -> cv::Mat { return img(getBlindLevelCoordinate()); };
            auto getBlindAmountImg(const cv::Mat& img) -> cv::Mat { return img(getBlindAmountCoordinate()); };
            auto getGameTimeImg(const cv::Mat& img) -> cv::Mat { return img(getGameTimeCoordinate()); };

            // All screen elements coordinates from top left corner (0, 0) in top left(x, y) cv::Rect(x, y, width, eight)
            virtual auto getFirstCardCoordinate() const -> const cv::Rect     = 0;
            virtual auto getSecondCardCoordinate() const -> const cv::Rect    = 0;
            virtual auto getPotCoordinate() const -> const cv::Rect           = 0;
            virtual auto getPrizePoolCoordinate() const -> const cv::Rect     = 0;
            virtual auto getBoardCard1Coordinate() const -> const cv::Rect    = 0;
            virtual auto getBoardCard2Coordinate() const -> const cv::Rect    = 0;
            virtual auto getBoardCard3Coordinate() const -> const cv::Rect    = 0;
            virtual auto getBoardCard4Coordinate() const -> const cv::Rect    = 0;
            virtual auto getBoardCard5Coordinate() const -> const cv::Rect    = 0;
            virtual auto getPlayer1NameCoordinate() const -> const cv::Rect   = 0;
            virtual auto getPlayer2NameCoordinate() const -> const cv::Rect   = 0;
            virtual auto getPlayer3NameCoordinate() const -> const cv::Rect   = 0;
            virtual auto getPlayer1ButtonCoordinate() const -> const cv::Rect = 0;
            virtual auto getPlayer2ButtonCoordinate() const -> const cv::Rect = 0;
            virtual auto getPlayer3ButtonCoordinate() const -> const cv::Rect = 0;
            virtual auto getPlayer1StackCoordinate() const -> const cv::Rect  = 0;
            virtual auto getPlayer2StackCoordinate() const -> const cv::Rect  = 0;
            virtual auto getPlayer3StackCoordinate() const -> const cv::Rect  = 0;
            virtual auto getPlayer1BetCoordinate() const -> const cv::Rect    = 0;
            virtual auto getPlayer2BetCoordinate() const -> const cv::Rect    = 0;
            virtual auto getPlayer3BetCoordinate() const -> const cv::Rect    = 0;
            virtual auto getPlayer2HandCoordinate() const -> const cv::Rect   = 0;
            virtual auto getPlayer3HandCoordinate() const -> const cv::Rect   = 0;
            virtual auto getBlindLevelCoordinate() const -> const cv::Rect    = 0;
            virtual auto getBlindAmountCoordinate() const -> const cv::Rect   = 0;
            virtual auto getGameTimeCoordinate() const -> const cv::Rect      = 0;

        private:
            windows_t _activeWindows;

            auto _parseActiveWindows() -> void;
            auto _rectToJson(const cv::Rect& rect) const -> json;
    };
}  // namespace Scraper
