#include "game_session/Session.hpp"

#include <logger/Logger.hpp>
#include <utilities/Image.hpp>
#include <utilities/Strings.hpp>
#include <utilities/Time.hpp>

namespace GameSession {
    using GameHandler::Hand;
    using GameHandler::invalid_player_name;
    using OCR::ExceptionWithImage;
    using Utilities::Image::writeImage;
    using Utilities::Strings::InvalidNumberException;

    using Logger = Logger::Quill;

    using enum RoundAction::ActionType;

    // @todo get window size from windowId if not provided
    Session::Session(std::string_view roomName, uint64_t windowId, windowSize_t windowSize) :
        _roomName(roomName), _windowId(windowId), _scraper(_roomName, windowSize), _ocr(OcrFactory::create(_roomName)), _game() {}

    auto Session::operator=(Session&& other) noexcept -> Session& {
        if (this != &other) {
            _game                    = std::move(other._game);
            _scraper                 = std::move(other._scraper);
            _ocr                     = std::move(other._ocr);
            _currentScreenshot       = std::move(other._currentScreenshot);
            _roomName                = std::move(other._roomName);
            _tickRate                = other._tickRate;
            _windowId                = other._windowId;
            _windowSize              = other._windowSize;
            _gameStage               = other._gameStage;
            _currentPlayerPlayingNum = other._currentPlayerPlayingNum;
            _currentAction           = other._currentAction;
        }

        return *this;
    }

    auto Session::run() -> void {
        _currentScreenshot = _getScreenshot();

        while (_gameStage != GameStages::ENDED) {
            DISPLAY_VIDEO("game", _scraper.getWindowElementsView(*_currentScreenshot));

            switch (_gameStage) {
                case GameStages::STARTING: _waitGameStart(*_currentScreenshot); break;
                case GameStages::GAME_INFO_SETUP: _harvestGameInfo(*_currentScreenshot); break;
                case GameStages::IN_PROGRESS:
                    if (!_game.getCurrentRound().isInProgress()) {
                        _determineGameOver();

                        if (!_game.isOver()) { _startRound(*_currentScreenshot); }
                    } else if (_isNextActionTriggered(*_currentScreenshot)) {
                        _trackCurrentRound(*_currentScreenshot);
                    }

                    break;
                case GameStages::ENDED: break;
            }

            std::this_thread::sleep_for(_tickRate);

            _currentScreenshot = _getScreenshot();
        }
    }

    // Used for testing by mocking this method
    auto Session::_getScreenshot() -> sharedConstMat_t { return _scraper.getScreenshot(_windowId); }

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
        int32_t amount = 0;
        // readPlayerBet may fail, so we keep the action until we can read the bet and reset the action to NONE in case of success
        if (_currentAction == NONE) { _currentAction = _ocr->readGameAction(_scraper.getPlayerActionImg(screenshot, playerNum)); }

        switch (_currentAction) {
            case FOLD: _game.getCurrentRound().fold(playerNum); break;
            case CHECK: _game.getCurrentRound().check(playerNum); break;
            case CALL:
                amount = _ocr->readPlayerBet(_scraper.getPlayerBetImg(screenshot, playerNum));
                _game.getCurrentRound().call(playerNum, amount);
                break;
            case BET:
                amount = _ocr->readPlayerBet(_scraper.getPlayerBetImg(screenshot, playerNum));
                _game.getCurrentRound().bet(playerNum, amount);
                break;
            case RAISE:
                amount = _ocr->readPlayerBet(_scraper.getPlayerBetImg(screenshot, playerNum));
                _game.getCurrentRound().raise(playerNum, amount);
                break;
            default: _currentAction = NONE;
        }

        if (amount > 0) {
            LOG_INFO(Logger::getLogger(), "Player {} {} {}", playerNum, _currentAction, amount);
        } else {
            LOG_INFO(Logger::getLogger(), "Player {} {}", playerNum, _currentAction);
        }

        _currentPlayerPlayingNum = _game.getCurrentRound().getNextPlayerNum(_currentPlayerPlayingNum);
        _lastWaitingActionImg    = _scraper.getPlayerActionImg(screenshot, _currentPlayerPlayingNum);
        _currentAction           = NONE;
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
            // @todo handle already started game
            if (_ocr->readBlindLevel(_scraper.getBlindLevelImg(screenshot)) > 1) { _game.setComplete(false); }
            // Get buy-in @todo read buy-in from game screen or from room config
            // Get prize pool
            // @todo _ocr->readPrizePool fails reading the prize pool
            // _game.setMultipliers(_ocr->readPrizePool(_scraper.getPrizePoolImg(screenshot)) / _game.getBuyIn());
            _game.setMultipliers(2);
            // Get player stacks
            _game.setInitialStack(_ocr->readAverageStack(_scraper.getAverageStackImg(screenshot)));
            // Get player names
            _game.init(_ocr->readPlayerName(_scraper.getPlayer1NameImg(screenshot)),
                       _ocr->readPlayerName(_scraper.getPlayer2NameImg(screenshot)),
                       _ocr->readPlayerName(_scraper.getPlayer3NameImg(screenshot)));
            // End the initialization
            LOG_INFO(Logger::getLogger(),
                     "Game information harvested successfully\n\tBuy-in: {}\n\tPrize pool: {}\n\tInitial stack: "
                     "{}\n\tPlayer 1: {}\n\tPlayer 2: {}\n\tPlayer 3: {}",
                     _game.getBuyIn(), _game.getBuyIn() * _game.getMultipliers(), _game.getInitialStack(),
                     _game.getPlayer1().getName(), _game.getPlayer2().getName(), _game.getPlayer3().getName());

            _gameStage = GameStages::IN_PROGRESS;
            // Start the 1st round
            _startRound(screenshot);
        } catch (const ExceptionWithImage& e) { LOG_DEBUG(Logger::getLogger(), "{}", e.what()); }
    }

    auto Session::_isNextActionTriggered(const cv::Mat& screenshot) -> bool {
        return !_ocr->isSimilar(_scraper.getPlayerActionImg(screenshot, _currentPlayerPlayingNum), _lastWaitingActionImg);
    }

    auto Session::_startRound(const cv::Mat& screenshot) -> void {
        // Get button position
        _assignButton(screenshot);
        // Get round blind level
        auto blinds = _ocr->readBlindRange(_scraper.getBlindAmountImg(screenshot));
        // Get hand
        auto firstCard  = _ocr->readCard(_scraper.getFirstCardImg(screenshot));
        auto secondCard = _ocr->readCard(_scraper.getSecondCardImg(screenshot));
        Hand hand       = {firstCard, secondCard};
        // Start round, _currentPlayerPlayingNum is set to the button position by _assignButton call
        LOG_INFO(Logger::getLogger(), "Starting new round: [blinds {}] [hand {}] [button {}]", blinds, hand, _currentPlayerPlayingNum);

        _game.newRound(blinds, hand, _currentPlayerPlayingNum);
    }

    auto Session::_trackCurrentRound(const cv::Mat& screenshot) -> void {
        try {
            _determinePlayerAction(screenshot, _currentPlayerPlayingNum);
        } catch (const ExceptionWithImage& e) {
            LOG_DEBUG(Logger::getLogger(), "{}", e.what());

            writeImage(e.getImage(), LOGS_DIR, e.getCategory());
        }
    }

    auto Session::_waitGameStart(const cv::Mat& screenshot) -> void {
        try {
            // Get hand
            auto firstCard  = _ocr->readCard(_scraper.getFirstCardImg(screenshot));
            auto secondCard = _ocr->readCard(_scraper.getSecondCardImg(screenshot));
            // Check if hand has been dealt, if not, wait
            if (firstCard.isUnknown() || secondCard.isUnknown()) { return; }

            auto buttonPosition = _getButtonPosition(*_currentScreenshot);

            LOG_INFO(Logger::getLogger(), "Game started, player {} is the button", buttonPosition);

            _gameStage = GameStages::GAME_INFO_SETUP;
        } catch (const CannotFindButtonException& e) {
            LOG_DEBUG(Logger::getLogger(), "Waiting game start, button not found");
        } catch (const ExceptionWithImage& e) { LOG_DEBUG(Logger::getLogger(), "Waiting game start, hand not distributed"); }
    }
}  // namespace GameSession