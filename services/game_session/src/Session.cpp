#include "game_session/Session.hpp"

#include <logger/Logger.hpp>

namespace GameSession {
    using GameHandler::invalid_player_name;

    using Logger = Logger::Quill;

    using enum RoundAction::ActionType;

    Session::Session(std::string_view roomName, uint64_t windowId) :
        _roomName(roomName), _windowId(windowId), _scraper(_roomName, {0, 0}), _ocr(OcrFactory::create(_roomName)), _game() {}

    auto Session::operator=(Session&& other) noexcept -> Session& {
        if (this != &other) {
            _game                    = std::move(other._game);
            _scraper                 = std::move(other._scraper);
            _tickRate                = other._tickRate;
            _windowId                = other._windowId;
            _gameStage               = other._gameStage;
            _currentPlayerPlayingNum = other._currentPlayerPlayingNum;
        }

        return *this;
    }

    auto Session::run() -> void {
        _getScreenshot().copyTo(_currentScreenshot);

        while (_gameStage != GameStages::ENDED) {
            switch (_gameStage) {
                case GameStages::STARTING: _waitGameStart(); break;
                case GameStages::GAME_INFO_SETUP: _harvestGameInfo(_currentScreenshot); break;
                case GameStages::IN_PROGRESS:
                    while (_game.getCurrentRound().isInProgress()) {
                        if (!_isNextActionTriggered(_currentScreenshot)) { continue; }

                        _trackCurrentRound(_currentScreenshot);
                    }

                    _determineGameOver();

                    if (!_game.isOver()) { _startRound(_currentScreenshot); }

                    break;
                case GameStages::ENDED: break;
            }

            std::this_thread::sleep_for(_tickRate);

            _getScreenshot().copyTo(_currentScreenshot);
        }
    }

    // Used for testing by mocking this method
    auto Session::_getScreenshot() -> cv::Mat { return _scraper.getScreenshot(_windowId); }

    auto Session::_assignButton(const cv::Mat& screenshot) -> void {
        _currentPlayerPlayingNum = _getButtonPosition(screenshot);
        _lastWaitingActionImg    = _scraper.getPlayerActionImg(screenshot, _currentPlayerPlayingNum);
    }

    auto Session::_determineGameOver() -> void {
        const auto& players = _game.getPlayers();

        auto winnerFinder = [](const Player& player) { return !player.isEliminated(); };

        if (std::count_if(players.begin(), players.end(), winnerFinder) == 1) {
            _game.end();
            _gameStage = GameStages::ENDED;
        }
    }

    auto Session::_determinePlayerAction(const cv::Mat& screenshot, uint32_t playerNum) -> void {
        auto action = _ocr->readGameAction(_scraper.getPlayerActionImg(screenshot, playerNum));

        switch (action) {
            case FOLD: _game.getCurrentRound().fold(playerNum); break;
            case CHECK: _game.getCurrentRound().check(playerNum); break;
            case CALL:
                _game.getCurrentRound().call(playerNum, _ocr->readPlayerBet(_scraper.getPlayerBetImg(screenshot, playerNum)));
                break;
            case BET:
                _game.getCurrentRound().bet(playerNum, _ocr->readPlayerBet(_scraper.getPlayerBetImg(screenshot, playerNum)));
                break;
            case RAISE:
                _game.getCurrentRound().raise(playerNum, _ocr->readPlayerBet(_scraper.getPlayerBetImg(screenshot, playerNum)));
                break;
            default: throw InvalidActionException(fmt::format("Invalid action detected `{}`", action));
        }
    }

    auto Session::_getButtonPosition(const cv::Mat& screenshot) -> int32_t {
        for (int32_t i = 1; i <= 3; i++) {
            if (!_ocr->isSimilar(_scraper.getPlayerButtonImg(screenshot, i), _ocr->getButtonImg())) { continue; }

            return i;
        }

        throw CannotFindButtonException("Could not determine button position");
    }

    /**
     * The _harvestGameInfo function is responsible for extracting game information from a given screenshot.
     *
     * It is called once at the initialization of the game.
     *
     * @param screenshot The screenshot of the game screen.
     * @return void
     */
    auto Session::_harvestGameInfo(const cv::Mat& screenshot) -> void {
        try {
            // Determine if game was already started
            // @todo use game duration to determine if game was already started
            if (_ocr->readBlindLevel(_scraper.getBlindLevelImg(screenshot)) > 1) { _game.setComplete(false); }
            // Get buy-in @todo read buy-in from game screen or from room config
            // Get prize pool
            _game.setMultipliers(_ocr->readPrizePool(_scraper.getPrizePoolImg(screenshot)) / _game.getBuyIn());
            // Get player stacks
            _game.setInitialStack(_ocr->readAverageStack(_scraper.getAverageStackImg(screenshot)));
            // Get player names
            _game.init(_ocr->readPlayerName(_scraper.getPlayer1NameImg(screenshot)),
                       _ocr->readPlayerName(_scraper.getPlayer2NameImg(screenshot)),
                       _ocr->readPlayerName(_scraper.getPlayer3NameImg(screenshot)));
            // End the initialization
            _gameStage = GameStages::IN_PROGRESS;
            // Start the 1st round
            _startRound(screenshot);
        } catch (const invalid_player_name& error) { LOG_INFO(Logger::getLogger(), "{}", error.what()); }
    }

    auto Session::_isNextActionTriggered(const cv::Mat& screenshot) -> bool {
        return _ocr->isSimilar(_scraper.getPlayerActionImg(screenshot, _currentPlayerPlayingNum), _lastWaitingActionImg);
    }

    auto Session::_startRound(const cv::Mat& screenshot) -> void {
        // Get button position
        _assignButton(screenshot);
        // Get round blind level
        auto blinds = _ocr->readBlindRange(_scraper.getBlindAmountImg(screenshot));
        // Get hand
        auto firstCard  = _ocr->readCard(_scraper.getFirstCardImg(screenshot));
        auto secondCard = _ocr->readCard(_scraper.getSecondCardImg(screenshot));

        _game.newRound(blinds, {firstCard, secondCard}, _currentPlayerPlayingNum);
    }

    auto Session::_trackCurrentRound(const cv::Mat& screenshot) -> void {
        try {
            _determinePlayerAction(screenshot, _currentPlayerPlayingNum);
        } catch (const InvalidActionException& error) { LOG_INFO(Logger::getLogger(), "{}", error.what()); }
    }

    auto Session::_waitGameStart() -> void {
        while (_gameStage == GameStages::STARTING) {
            try {
                _getButtonPosition(_currentScreenshot);

                _gameStage = GameStages::GAME_INFO_SETUP;
            } catch (const CannotFindButtonException& e) {
                std::this_thread::sleep_for(_tickRate);

                _getScreenshot().copyTo(_currentScreenshot);
            }
        }
    }
}  // namespace GameSession