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
    using windowSize_t = std::pair<int32_t, int32_t>;

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

    class Model {
        public:
            using windows_t = std::unordered_map<uint64_t, WindowInfo>;

            Model(std::string_view roomName, windowSize_t windowSize) : _windowSize(std::move(windowSize)), _roomName(roomName){};
            Model(const Model& other) = default;
            Model(Model&& other) noexcept { *this = std::move(other); };

            virtual ~Model() = default;

            auto operator=(const Model& other) -> Model& = default;
            auto operator=(Model&& other) noexcept -> Model&;

            auto getWindowSize() const -> const windowSize_t& { return _windowSize; }
            auto getRoomName() const -> const std::string& { return _roomName; }
            auto getFirstCardCoord() const -> const cv::Rect& { return _firstCardCoord; }
            auto getSecondCardCoord() const -> const cv::Rect& { return _secondCardCoord; }
            auto getPotCoord() const -> const cv::Rect& { return _potCoord; }
            auto getPrizePoolCoord() const -> const cv::Rect& { return _prizePoolCoord; }
            auto getBoardCard1Coord() const -> const cv::Rect& { return _boardCard1Coord; }
            auto getBoardCard2Coord() const -> const cv::Rect& { return _boardCard2Coord; }
            auto getBoardCard3Coord() const -> const cv::Rect& { return _boardCard3Coord; }
            auto getBoardCard4Coord() const -> const cv::Rect& { return _boardCard4Coord; }
            auto getBoardCard5Coord() const -> const cv::Rect& { return _boardCard5Coord; }
            auto getPlayer1NameCoord() const -> const cv::Rect& { return _player1NameCoord; }
            auto getPlayer2NameCoord() const -> const cv::Rect& { return _player2NameCoord; }
            auto getPlayer3NameCoord() const -> const cv::Rect& { return _player3NameCoord; }
            auto getPlayer1ButtonCoord() const -> const cv::Rect& { return _player1ButtonCoord; }
            auto getPlayer2ButtonCoord() const -> const cv::Rect& { return _player2ButtonCoord; }
            auto getPlayer3ButtonCoord() const -> const cv::Rect& { return _player3ButtonCoord; }
            auto getPlayer1StackCoord() const -> const cv::Rect& { return _player1StackCoord; }
            auto getPlayer2StackCoord() const -> const cv::Rect& { return _player2StackCoord; }
            auto getPlayer3StackCoord() const -> const cv::Rect& { return _player3StackCoord; }
            auto getPlayer1BetCoord() const -> const cv::Rect& { return _player1BetCoord; }
            auto getPlayer2BetCoord() const -> const cv::Rect& { return _player2BetCoord; }
            auto getPlayer3BetCoord() const -> const cv::Rect& { return _player3BetCoord; }
            auto getPlayer2HandCoord() const -> const cv::Rect& { return _player2HandCoord; }
            auto getPlayer3HandCoord() const -> const cv::Rect& { return _player3HandCoord; }
            auto getPlayer2ActionCoord() const -> const cv::Rect& { return _player2ActionCoord; }
            auto getPlayer3ActionCoord() const -> const cv::Rect& { return _player3ActionCoord; }
            auto getBlindLevelCoord() const -> const cv::Rect& { return _blindLevelCoord; }
            auto getBlindAmountCoord() const -> const cv::Rect& { return _blindAmountCoord; }
            auto getGameTimeCoord() const -> const cv::Rect& { return _gameTimeCoord; }

            void setWindowSize(const windowSize_t& windowSize) { _windowSize = windowSize; }
            void setRoomName(const std::string& roomName) { _roomName = roomName; }
            void setFirstCardCoord(const cv::Rect& firstCardCoord) { _firstCardCoord = firstCardCoord; }
            void setSecondCardCoord(const cv::Rect& secondCardCoord) { _secondCardCoord = secondCardCoord; }
            void setPotCoord(const cv::Rect& potCoord) { _potCoord = potCoord; }
            void setPrizePoolCoord(const cv::Rect& prizePoolCoord) { _prizePoolCoord = prizePoolCoord; }
            void setBoardCard1Coord(const cv::Rect& boardCard1Coord) { _boardCard1Coord = boardCard1Coord; }
            void setBoardCard2Coord(const cv::Rect& boardCard2Coord) { _boardCard2Coord = boardCard2Coord; }
            void setBoardCard3Coord(const cv::Rect& boardCard3Coord) { _boardCard3Coord = boardCard3Coord; }
            void setBoardCard4Coord(const cv::Rect& boardCard4Coord) { _boardCard4Coord = boardCard4Coord; }
            void setBoardCard5Coord(const cv::Rect& boardCard5Coord) { _boardCard5Coord = boardCard5Coord; }
            void setPlayer1NameCoord(const cv::Rect& player1NameCoord) { _player1NameCoord = player1NameCoord; }
            void setPlayer2NameCoord(const cv::Rect& player2NameCoord) { _player2NameCoord = player2NameCoord; }
            void setPlayer3NameCoord(const cv::Rect& player3NameCoord) { _player3NameCoord = player3NameCoord; }
            void setPlayer1ButtonCoord(const cv::Rect& player1ButtonCoord) { _player1ButtonCoord = player1ButtonCoord; }
            void setPlayer2ButtonCoord(const cv::Rect& player2ButtonCoord) { _player2ButtonCoord = player2ButtonCoord; }
            void setPlayer3ButtonCoord(const cv::Rect& player3ButtonCoord) { _player3ButtonCoord = player3ButtonCoord; }
            void setPlayer1StackCoord(const cv::Rect& player1StackCoord) { _player1StackCoord = player1StackCoord; }
            void setPlayer2StackCoord(const cv::Rect& player2StackCoord) { _player2StackCoord = player2StackCoord; }
            void setPlayer3StackCoord(const cv::Rect& player3StackCoord) { _player3StackCoord = player3StackCoord; }
            void setPlayer1BetCoord(const cv::Rect& player1BetCoord) { _player1BetCoord = player1BetCoord; }
            void setPlayer2BetCoord(const cv::Rect& player2BetCoord) { _player2BetCoord = player2BetCoord; }
            void setPlayer3BetCoord(const cv::Rect& player3BetCoord) { _player3BetCoord = player3BetCoord; }
            void setPlayer2HandCoord(const cv::Rect& player2HandCoord) { _player2HandCoord = player2HandCoord; }
            void setPlayer3HandCoord(const cv::Rect& player3HandCoord) { _player3HandCoord = player3HandCoord; }
            void setPlayer2ActionCoord(const cv::Rect& player2ActionCoord) { _player2ActionCoord = player2ActionCoord; }
            void setPlayer3ActionCoord(const cv::Rect& player3ActionCoord) { _player2ActionCoord = player3ActionCoord; }
            void setBlindLevelCoord(const cv::Rect& blindLevelCoord) { _blindLevelCoord = blindLevelCoord; }
            void setBlindAmountCoord(const cv::Rect& blindAmountCoord) { _blindAmountCoord = blindAmountCoord; }
            void setGameTimeCoord(const cv::Rect& gameTimeCoord) { _gameTimeCoord = gameTimeCoord; }

            /**
             *              Players positions on the table
             *
             *          Player 2  __________________  Player 3
             *                   |                  |
             *                   |                  |
             *                   |__________________|
             *                         Player 1
             */
            auto getFirstCardImg(const cv::Mat& img) -> cv::Mat { return img(getFirstCardCoord()); };
            auto getSecondCardImg(const cv::Mat& img) -> cv::Mat { return img(getSecondCardCoord()); };
            auto getPotImg(const cv::Mat& img) -> cv::Mat { return img(getPotCoord()); };
            auto getPrizePoolImg(const cv::Mat& img) -> cv::Mat { return img(getPrizePoolCoord()); };
            auto getBoardCard1Img(const cv::Mat& img) -> cv::Mat { return img(getBoardCard1Coord()); };
            auto getBoardCard2Img(const cv::Mat& img) -> cv::Mat { return img(getBoardCard2Coord()); };
            auto getBoardCard3Img(const cv::Mat& img) -> cv::Mat { return img(getBoardCard3Coord()); };
            auto getBoardCard4Img(const cv::Mat& img) -> cv::Mat { return img(getBoardCard4Coord()); };
            auto getBoardCard5Img(const cv::Mat& img) -> cv::Mat { return img(getBoardCard5Coord()); };
            auto getPlayer1NameImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer1NameCoord()); };
            auto getPlayer2NameImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer2NameCoord()); };
            auto getPlayer3NameImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer3NameCoord()); };
            auto getPlayer1ButtonImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer1ButtonCoord()); };
            auto getPlayer2ButtonImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer2ButtonCoord()); };
            auto getPlayer3ButtonImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer3ButtonCoord()); };
            auto getPlayer1StackImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer1StackCoord()); };
            auto getPlayer2StackImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer2StackCoord()); };
            auto getPlayer3StackImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer3StackCoord()); };
            auto getPlayer1BetImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer1BetCoord()); };
            auto getPlayer2BetImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer2BetCoord()); };
            auto getPlayer3BetImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer3BetCoord()); };
            auto getPlayer2CardsImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer2HandCoord()); };
            auto getPlayer3CardsImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer3HandCoord()); };
            auto getPlayer2ActionImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer2ActionCoord()); };
            auto getPlayer3ActionImg(const cv::Mat& img) -> cv::Mat { return img(getPlayer3ActionCoord()); };
            auto getBlindLevelImg(const cv::Mat& img) -> cv::Mat { return img(getBlindLevelCoord()); };
            auto getBlindAmountImg(const cv::Mat& img) -> cv::Mat { return img(getBlindAmountCoord()); };
            auto getGameTimeImg(const cv::Mat& img) -> cv::Mat { return img(getGameTimeCoord()); };

            // Convenient methods to get coordinates with a player number
            auto getPlayerHandCoord(uint32_t playerNum) const -> const cv::Rect&;
            auto getPlayerActionCoord(uint32_t playerNum) const -> const cv::Rect&;
            auto getPlayerBetCoord(uint32_t playerNum) const -> const cv::Rect&;
            auto getPlayerStackCoord(uint32_t playerNum) const -> const cv::Rect&;
            auto getPlayerButtonCoord(uint32_t playerNum) const -> const cv::Rect&;
            // For the image extraction
            auto getPlayerCardsImg(const cv::Mat& img, uint32_t playerNum) -> cv::Mat { return img(getPlayerHandCoord(playerNum)); }
            auto getPlayerActionImg(const cv::Mat& img, uint32_t playerNum) -> cv::Mat { return img(getPlayerActionCoord(playerNum)); }
            auto getPlayerBetImg(const cv::Mat& img, uint32_t playerNum) -> cv::Mat { return img(getPlayerBetCoord(playerNum)); }
            auto getPlayerStackImg(const cv::Mat& img, uint32_t playerNum) -> cv::Mat { return img(getPlayerStackCoord(playerNum)); }
            auto getPlayerButtonImg(const cv::Mat& img, uint32_t playerNum) -> cv::Mat { return img(getPlayerButtonCoord(playerNum)); }

            auto getActiveWindows() -> windows_t;
            auto getScreenshot(uint64_t windowId) -> cv::Mat;
            auto getWindowElementsView(const cv::Mat& img) -> cv::Mat;

            [[nodiscard]] auto toJson() const -> json;

            auto loadFromJson(const json& json) -> void;

        private:
            windows_t    _activeWindows;
            windowSize_t _windowSize;
            std::string  _roomName;

            // All screen elements coordinates from attributes as cv::Rect object
            cv::Rect _firstCardCoord;
            cv::Rect _secondCardCoord;
            cv::Rect _potCoord;
            cv::Rect _prizePoolCoord;
            cv::Rect _boardCard1Coord;
            cv::Rect _boardCard2Coord;
            cv::Rect _boardCard3Coord;
            cv::Rect _boardCard4Coord;
            cv::Rect _boardCard5Coord;
            cv::Rect _player1NameCoord;
            cv::Rect _player2NameCoord;
            cv::Rect _player3NameCoord;
            cv::Rect _player1ButtonCoord;
            cv::Rect _player2ButtonCoord;
            cv::Rect _player3ButtonCoord;
            cv::Rect _player1StackCoord;
            cv::Rect _player2StackCoord;
            cv::Rect _player3StackCoord;
            cv::Rect _player1BetCoord;
            cv::Rect _player2BetCoord;
            cv::Rect _player3BetCoord;
            cv::Rect _player2HandCoord;
            cv::Rect _player3HandCoord;
            cv::Rect _player2ActionCoord;
            cv::Rect _player3ActionCoord;
            cv::Rect _blindLevelCoord;
            cv::Rect _blindAmountCoord;
            cv::Rect _gameTimeCoord;

            auto _parseActiveWindows() -> void;
            auto _rectToJson(const cv::Rect& rect) const -> json;
            auto _jsonToRect(const json& json) const -> cv::Rect;
    };
}  // namespace Scraper
