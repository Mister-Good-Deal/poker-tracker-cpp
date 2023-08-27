#pragma once

#include <string>
#include <unordered_map>
#include <utility>

#include <opencv4/opencv2/opencv.hpp>

#include <nlohmann/json.hpp>

#include <logger/Logger.hpp>

#ifdef _WIN32

    #include <windows.h>

#elif __linux__

    #include <X11/Xlib.h>
    #include <X11/Xos.h>
    #include <X11/Xutil.h>

#endif
namespace Scraper {
    using json         = nlohmann::json;
    using windowSize_t = cv::Size_<int32_t>;

    static constexpr std::string_view MODEL_PATH_FORMAT = "{dir}/models/{size}.json";

    struct WindowInfo {
            std::string title;
            uint64_t    id;
#ifdef _WIN32
            HWND ref;

            WindowInfo(std::string_view title, HWND ref)
              : title(title)
              , id(std::hash<std::string_view> {}(title), ref(ref)) {}
#elif __linux__
            Window ref;

            WindowInfo(std::string_view title, Window ref)
              : title(title)
              , id(ref)
              , ref(ref) {}
#endif
    };

    class Model {
        public:
            using windows_t        = std::unordered_map<uint64_t, WindowInfo>;
            using sharedConstMat_t = std::shared_ptr<const cv::Mat>;

            Model(std::string_view roomName, windowSize_t windowSize);
            Model(const Model& other) = default;
            Model(Model&& other) noexcept { *this = std::move(other); };

            virtual ~Model() = default;

            auto operator=(const Model& other) -> Model& = default;
            auto operator=(Model&& other) noexcept -> Model&;

            auto getAverageStackCoord() const -> const cv::Rect& { return _averageStackCoord; }
            auto getBlindAmountCoord() const -> const cv::Rect& { return _blindAmountCoord; }
            auto getBlindLevelCoord() const -> const cv::Rect& { return _blindLevelCoord; }
            auto getBlindLevelTimeCoord() const -> const cv::Rect& { return _blindLevelTimeCoord; }
            auto getBoardCard1Coord() const -> const cv::Rect& { return _boardCard1Coord; }
            auto getBoardCard2Coord() const -> const cv::Rect& { return _boardCard2Coord; }
            auto getBoardCard3Coord() const -> const cv::Rect& { return _boardCard3Coord; }
            auto getBoardCard4Coord() const -> const cv::Rect& { return _boardCard4Coord; }
            auto getBoardCard5Coord() const -> const cv::Rect& { return _boardCard5Coord; }
            auto getFirstCardCoord() const -> const cv::Rect& { return _firstCardCoord; }
            auto getSecondCardCoord() const -> const cv::Rect& { return _secondCardCoord; }
            auto getPlayer1ActionCoord() const -> const cv::Rect& { return _player1ActionCoord; }
            auto getPlayer1BetCoord() const -> const cv::Rect& { return _player1BetCoord; }
            auto getPlayer1ButtonCoord() const -> const cv::Rect& { return _player1ButtonCoord; }
            auto getPlayer1NameCoord() const -> const cv::Rect& { return _player1NameCoord; }
            auto getPlayer1StackCoord() const -> const cv::Rect& { return _player1StackCoord; }
            auto getPlayer2ActionCoord() const -> const cv::Rect& { return _player2ActionCoord; }
            auto getPlayer2BetCoord() const -> const cv::Rect& { return _player2BetCoord; }
            auto getPlayer2ButtonCoord() const -> const cv::Rect& { return _player2ButtonCoord; }
            auto getPlayer2CardsCoord() const -> const cv::Rect& { return _player2CardsCoord; }
            auto getPlayer2HandCoord() const -> const cv::Rect& { return _player2HandCoord; }
            auto getPlayer2NameCoord() const -> const cv::Rect& { return _player2NameCoord; }
            auto getPlayer2StackCoord() const -> const cv::Rect& { return _player2StackCoord; }
            auto getPlayer3ActionCoord() const -> const cv::Rect& { return _player3ActionCoord; }
            auto getPlayer3BetCoord() const -> const cv::Rect& { return _player3BetCoord; }
            auto getPlayer3ButtonCoord() const -> const cv::Rect& { return _player3ButtonCoord; }
            auto getPlayer3CardsCoord() const -> const cv::Rect& { return _player3CardsCoord; }
            auto getPlayer3HandCoord() const -> const cv::Rect& { return _player3HandCoord; }
            auto getPlayer3NameCoord() const -> const cv::Rect& { return _player3NameCoord; }
            auto getPlayer3StackCoord() const -> const cv::Rect& { return _player3StackCoord; }
            auto getPotCoord() const -> const cv::Rect& { return _potCoord; }
            auto getPrizePoolCoord() const -> const cv::Rect& { return _prizePoolCoord; }
            auto getRoomName() const -> const std::string& { return _roomName; }
            auto getWindowSize() const -> const windowSize_t& { return _windowSize; }

            void setAverageStackCoord(const cv::Rect& imgROI) { _averageStackCoord = imgROI; }
            void setBlindAmountCoord(const cv::Rect& imgROI) { _blindAmountCoord = imgROI; }
            void setBlindLevelCoord(const cv::Rect& imgROI) { _blindLevelCoord = imgROI; }
            void setBlindLevelTimeCoord(const cv::Rect& imgROI) { _blindLevelTimeCoord = imgROI; }
            void setBoardCard1Coord(const cv::Rect& imgROI) { _boardCard1Coord = imgROI; }
            void setBoardCard2Coord(const cv::Rect& imgROI) { _boardCard2Coord = imgROI; }
            void setBoardCard3Coord(const cv::Rect& imgROI) { _boardCard3Coord = imgROI; }
            void setBoardCard4Coord(const cv::Rect& imgROI) { _boardCard4Coord = imgROI; }
            void setBoardCard5Coord(const cv::Rect& imgROI) { _boardCard5Coord = imgROI; }
            void setFirstCardCoord(const cv::Rect& imgROI) { _firstCardCoord = imgROI; }
            void setSecondCardCoord(const cv::Rect& imgROI) { _secondCardCoord = imgROI; }
            void setPlayer1ActionCoord(const cv::Rect& imgROI) { _player1ActionCoord = imgROI; }
            void setPlayer1BetCoord(const cv::Rect& imgROI) { _player1BetCoord = imgROI; }
            void setPlayer1ButtonCoord(const cv::Rect& imgROI) { _player1ButtonCoord = imgROI; }
            void setPlayer1NameCoord(const cv::Rect& imgROI) { _player1NameCoord = imgROI; }
            void setPlayer1StackCoord(const cv::Rect& imgROI) { _player1StackCoord = imgROI; }
            void setPlayer2ActionCoord(const cv::Rect& imgROI) { _player2ActionCoord = imgROI; }
            void setPlayer2BetCoord(const cv::Rect& imgROI) { _player2BetCoord = imgROI; }
            void setPlayer2ButtonCoord(const cv::Rect& imgROI) { _player2ButtonCoord = imgROI; }
            void setPlayer2CardsCoord(const cv::Rect& imgROI) { _player2CardsCoord = imgROI; }
            void setPlayer2HandCoord(const cv::Rect& imgROI) { _player2HandCoord = imgROI; }
            void setPlayer2NameCoord(const cv::Rect& imgROI) { _player2NameCoord = imgROI; }
            void setPlayer2StackCoord(const cv::Rect& imgROI) { _player2StackCoord = imgROI; }
            void setPlayer3ActionCoord(const cv::Rect& imgROI) { _player3ActionCoord = imgROI; }
            void setPlayer3BetCoord(const cv::Rect& imgROI) { _player3BetCoord = imgROI; }
            void setPlayer3ButtonCoord(const cv::Rect& imgROI) { _player3ButtonCoord = imgROI; }
            void setPlayer3CardsCoord(const cv::Rect& imgROI) { _player3CardsCoord = imgROI; }
            void setPlayer3HandCoord(const cv::Rect& imgROI) { _player3HandCoord = imgROI; }
            void setPlayer3NameCoord(const cv::Rect& imgROI) { _player3NameCoord = imgROI; }
            void setPlayer3StackCoord(const cv::Rect& imgROI) { _player3StackCoord = imgROI; }
            void setPotCoord(const cv::Rect& imgROI) { _potCoord = imgROI; }
            void setPrizePoolCoord(const cv::Rect& imgROI) { _prizePoolCoord = imgROI; }
            void setRoomName(const std::string& roomName) { _roomName = roomName; }
            void setWindowSize(const windowSize_t& windowSize) { _windowSize = windowSize; }

            /**
             *              Players positions on the table
             *
             *          Player 2  __________________  Player 3
             *                   |                  |
             *                   |                  |
             *                   |__________________|
             *                         Player 1
             */
            auto getFirstCardImg(const cv::Mat& img) const -> const cv::Mat { return img(getFirstCardCoord()); };
            auto getSecondCardImg(const cv::Mat& img) const -> const cv::Mat { return img(getSecondCardCoord()); };
            auto getAverageStackImg(const cv::Mat& img) const -> const cv::Mat { return img(getAverageStackCoord()); };
            auto getBlindAmountImg(const cv::Mat& img) const -> const cv::Mat { return img(getBlindAmountCoord()); };
            auto getBlindLevelImg(const cv::Mat& img) const -> const cv::Mat { return img(getBlindLevelCoord()); };
            auto getBlindLevelTimeImg(const cv::Mat& img) const -> const cv::Mat { return img(getBlindLevelTimeCoord()); };
            auto getBoardCard1Img(const cv::Mat& img) const -> const cv::Mat { return img(getBoardCard1Coord()); };
            auto getBoardCard2Img(const cv::Mat& img) const -> const cv::Mat { return img(getBoardCard2Coord()); };
            auto getBoardCard3Img(const cv::Mat& img) const -> const cv::Mat { return img(getBoardCard3Coord()); };
            auto getBoardCard4Img(const cv::Mat& img) const -> const cv::Mat { return img(getBoardCard4Coord()); };
            auto getBoardCard5Img(const cv::Mat& img) const -> const cv::Mat { return img(getBoardCard5Coord()); };
            auto getPlayer1ActionImg(const cv::Mat& img) const -> const cv::Mat { return img(getPlayer1ActionCoord()); };
            auto getPlayer1BetImg(const cv::Mat& img) const -> const cv::Mat { return img(getPlayer1BetCoord()); };
            auto getPlayer1ButtonImg(const cv::Mat& img) const -> const cv::Mat { return img(getPlayer1ButtonCoord()); };
            auto getPlayer1NameImg(const cv::Mat& img) const -> const cv::Mat { return img(getPlayer1NameCoord()); };
            auto getPlayer1StackImg(const cv::Mat& img) const -> const cv::Mat { return img(getPlayer1StackCoord()); };
            auto getPlayer2ActionImg(const cv::Mat& img) const -> const cv::Mat { return img(getPlayer2ActionCoord()); };
            auto getPlayer2BetImg(const cv::Mat& img) const -> const cv::Mat { return img(getPlayer2BetCoord()); };
            auto getPlayer2ButtonImg(const cv::Mat& img) const -> const cv::Mat { return img(getPlayer2ButtonCoord()); };
            auto getPlayer2CardsImg(const cv::Mat& img) const -> const cv::Mat { return img(getPlayer2HandCoord()); };
            auto getPlayer2NameImg(const cv::Mat& img) const -> const cv::Mat { return img(getPlayer2NameCoord()); };
            auto getPlayer2StackImg(const cv::Mat& img) const -> const cv::Mat { return img(getPlayer2StackCoord()); };
            auto getPlayer3ActionImg(const cv::Mat& img) const -> const cv::Mat { return img(getPlayer3ActionCoord()); };
            auto getPlayer3BetImg(const cv::Mat& img) const -> const cv::Mat { return img(getPlayer3BetCoord()); };
            auto getPlayer3ButtonImg(const cv::Mat& img) const -> const cv::Mat { return img(getPlayer3ButtonCoord()); };
            auto getPlayer3CardsImg(const cv::Mat& img) const -> const cv::Mat { return img(getPlayer3HandCoord()); };
            auto getPlayer3NameImg(const cv::Mat& img) const -> const cv::Mat { return img(getPlayer3NameCoord()); };
            auto getPlayer3StackImg(const cv::Mat& img) const -> const cv::Mat { return img(getPlayer3StackCoord()); };
            auto getPotImg(const cv::Mat& img) const -> const cv::Mat { return img(getPotCoord()); };
            auto getPrizePoolImg(const cv::Mat& img) const -> const cv::Mat { return img(getPrizePoolCoord()); };

            // Convenient methods to get coordinates with a player number
            auto getPlayerActionCoord(int32_t playerNum) const -> const cv::Rect&;
            auto getPlayerBetCoord(int32_t playerNum) const -> const cv::Rect&;
            auto getPlayerButtonCoord(int32_t playerNum) const -> const cv::Rect&;
            auto getPlayerCardsCoord(int32_t playerNum) const -> const cv::Rect&;
            auto getPlayerHandCoord(int32_t playerNum) const -> const cv::Rect&;
            auto getPlayerStackCoord(int32_t playerNum) const -> const cv::Rect&;
            // For the image extraction
            auto getPlayerActionImg(const cv::Mat& img, int32_t num) -> const cv::Mat { return img(getPlayerActionCoord(num)); }
            auto getPlayerBetImg(const cv::Mat& img, int32_t num) -> const cv::Mat { return img(getPlayerBetCoord(num)); }
            auto getPlayerButtonImg(const cv::Mat& img, int32_t num) -> const cv::Mat { return img(getPlayerButtonCoord(num)); }
            auto getPlayerCardsImg(const cv::Mat& img, int32_t num) -> const cv::Mat { return img(getPlayerCardsCoord(num)); }
            auto getPlayerHandImg(const cv::Mat& img, int32_t num) -> const cv::Mat { return img(getPlayerHandCoord(num)); }
            auto getPlayerStackImg(const cv::Mat& img, int32_t num) -> const cv::Mat { return img(getPlayerStackCoord(num)); }

            auto getActiveWindows() -> windows_t;
            auto getScreenshot(uint64_t windowId) -> sharedConstMat_t;
            auto getWindowElementsView(const cv::Mat& img) const -> const cv::Mat;

            [[nodiscard]] auto toJson() const -> json;

            auto loadFromJson(const json& json) -> void;

        private:
            windows_t    _activeWindows;
            windowSize_t _windowSize;
            std::string  _roomName;

            // All screen elements coordinates from attributes as cv::Rect object
            cv::Rect _firstCardCoord;
            cv::Rect _secondCardCoord;
            cv::Rect _averageStackCoord;
            cv::Rect _blindAmountCoord;
            cv::Rect _blindLevelCoord;
            cv::Rect _blindLevelTimeCoord;
            cv::Rect _boardCard1Coord;
            cv::Rect _boardCard2Coord;
            cv::Rect _boardCard3Coord;
            cv::Rect _boardCard4Coord;
            cv::Rect _boardCard5Coord;
            cv::Rect _player1ActionCoord;
            cv::Rect _player1BetCoord;
            cv::Rect _player1ButtonCoord;
            cv::Rect _player1NameCoord;
            cv::Rect _player1StackCoord;
            cv::Rect _player2ActionCoord;
            cv::Rect _player2BetCoord;
            cv::Rect _player2ButtonCoord;
            cv::Rect _player2CardsCoord;
            cv::Rect _player2HandCoord;
            cv::Rect _player2NameCoord;
            cv::Rect _player2StackCoord;
            cv::Rect _player3ActionCoord;
            cv::Rect _player3BetCoord;
            cv::Rect _player3ButtonCoord;
            cv::Rect _player3CardsCoord;
            cv::Rect _player3HandCoord;
            cv::Rect _player3NameCoord;
            cv::Rect _player3StackCoord;
            cv::Rect _potCoord;
            cv::Rect _prizePoolCoord;

            auto _parseActiveWindows() -> void;
            auto _rectToJson(const cv::Rect& rect) const -> json;
            auto _jsonToRect(const json& json) const -> cv::Rect;
            auto _loadDefaultModel() -> void;
    };
}  // namespace Scraper

// Custom fmt formatter for windowSize_t
namespace fmt {
    template<> struct formatter<Scraper::windowSize_t> : formatter<string_view> {
            template<typename FormatContext> auto format(const Scraper::windowSize_t& size, FormatContext& ctx) const {
                return fmt::format_to(ctx.out(), "{}x{}", size.width, size.height);
            }
    };
}  // namespace fmt
