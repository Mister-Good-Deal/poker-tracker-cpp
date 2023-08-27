#include "game_session/Session.hpp"

#include <logger/Logger.hpp>
#include <utilities/Image.hpp>
#include <utilities/Strings.hpp>
#include <utilities/Time.hpp>

namespace GameSession {
    using GameHandler::Hand;
    using GameHandler::invalid_player_name;
    using OCR::CannotReadBoardCardImageException;
    using OCR::CannotReadGameActionImageException;
    using OCR::CannotReadPlayerBetImageException;
    using OCR::CannotReadPlayerCardImageException;
    using OCR::ExceptionWithImage;
    using Utilities::Image::writeLogGameImage;
    using Utilities::Image::writeLogPlayerImage;
    using Utilities::Strings::InvalidNumberException;

    using Logger = Logger::Quill;

    using enum RoundAction::ActionType;
    using enum Round::Street;
    using enum GameStages;

    // @todo get window size from windowId if not provided
    Session::Session(std::string_view roomName, uint64_t windowId, windowSize_t windowSize)
      : _roomName(roomName)
      , _windowId(windowId)
      , _scraper(_roomName, windowSize)
      , _ocr(OcrFactory::create(_roomName))
      , _game() {}

    auto Session::operator=(Session&& other) noexcept -> Session& {
        if (this != &other) {
            _game              = std::move(other._game);
            _scraper           = std::move(other._scraper);
            _ocr               = std::move(other._ocr);
            _currentScreenshot = std::move(other._currentScreenshot);
            _roomName          = std::move(other._roomName);
            _tickRate          = other._tickRate;
            _windowId          = other._windowId;
            _windowSize        = other._windowSize;
            _gameStage         = other._gameStage;
            _currentPlayerNum  = other._currentPlayerNum;
            _currentAction     = other._currentAction;
        }

        return *this;
    }

    auto Session::run() -> void {
        _currentScreenshot = _getScreenshot();

        while (_gameStage != ENDED) {
            DISPLAY_VIDEO("game", _scraper.getWindowElementsView(*_currentScreenshot));

            auto& round = _game.getCurrentRound();

            switch (_gameStage) {
                case STARTING: _waitGameStart(*_currentScreenshot); [[fallthrough]];
                case GAME_INFO_SETUP: _harvestGameInfo(*_currentScreenshot); [[fallthrough]];
                case WAITING_NEW_ROUND: _waitNewRound(*_currentScreenshot); break;
                case ROUND_IN_PROGRESS:
                    if (round.waitingShowdown()) {
                        if (!_showdownTriggered) {
                            _setShowdownComparisonCards(*_currentScreenshot);
                            _showdownTriggered = true;
                        }

                        _waitShowdown(*_currentScreenshot);
                    } else {
                        _getStreetCards(*_currentScreenshot, round);  // Always get sure to get the street cards
                        // @todo verify the last street card(s) (could be misread on certain frames)

                        if ((_actionTriggered && _currentAction != NONE) || _isNextActionTriggered(*_currentScreenshot)) {
                            _actionTriggered = true;
                            _trackCurrentRound(*_currentScreenshot);
                        }
                    }

                    break;
                case ENDED: LOG_DEBUG(Logger::getLogger(), "Game ended\n{}", _game.toJson().dump(4)); break;
            }

            std::this_thread::sleep_for(_tickRate);

            _currentScreenshot = _getScreenshot();
        }
    }

    // Used for testing by mocking this method
    auto Session::_getScreenshot() -> sharedConstMat_t { return _scraper.getScreenshot(_windowId); }

    auto Session::_determineGameOver() -> void {
        const auto& players = _game.getPlayers();

        auto winnerFinder = [](const Player& player) { return !player.isEliminated(); };

        if (std::count_if(players.begin(), players.end(), winnerFinder) == 1) {
            _game.end();
            _gameStage = ENDED;
        }
    }

    auto Session::_determinePlayerAction(const cv::Mat& screenshot, const cv::Mat& actionImg, int32_t playerNum) -> void {
        auto& round = _game.getCurrentRound();
        // readPlayerBet may fail, so we keep the action until we can read the bet and reset the action to NONE in case of success
        if (_currentAction == NONE) {  // @todo check if this is needed
            try {
                _currentAction = _ocr->readGameAction(actionImg);
            } catch (const CannotReadGameActionImageException& e) {
                auto playerStackImg = _scraper.getPlayerStackImg(screenshot, playerNum);
                // Special case of all in, we can't read the action, so we check if the player is all in or if his stack changed
                // @todo review this
                if (_ocr->isAllIn(playerStackImg) || _ocr->readPlayerStack(playerStackImg) != round.getCurrentPlayerStack(playerNum)) {
                    _currentAction = CALL;
                } else {
                    throw;
                }
            }
        }

        if (_currentAction == BET || _currentAction == RAISE) {
            _playerBetImg.setCurrentImg(screenshot, playerNum);

            if (!_playerBetImg.isChecked(playerNum)) {
                LOG_DEBUG(Logger::getLogger(), "Player {} bet not checked", playerNum);
                return;
            }
        }

        switch (_currentAction) {
            case FOLD: round.fold(playerNum); break;
            case CHECK: round.check(playerNum); break;
            case CALL: round.call(playerNum); break;
            case BET: round.bet(playerNum, _ocr->readPlayerBet(_playerBetImg.getImg(playerNum))); break;
            case RAISE: round.raise(playerNum, _ocr->readPlayerBet(_playerBetImg.getImg(playerNum))); break;
            default: throw std::runtime_error("Unknown action");  // Should never happen
        }

        LOG_INFO(Logger::getLogger(), "{}", round.getLastAction());

        _currentPlayerNum     = round.getCurrentPlayerNum();
        _lastWaitingActionImg = _scraper.getPlayerActionImg(screenshot, _currentPlayerNum);
        _currentAction        = NONE;
        _actionTriggered      = false;
    }

    auto Session::_getButtonPosition(const cv::Mat& screenshot) -> int32_t {
        for (int32_t i = 1; i <= 3; i++) {
            if (!isSimilar(_scraper.getPlayerButtonImg(screenshot, i), _ocr->getButtonImg())) { continue; }

            return i;
        }

        throw CannotFindButtonException();
    }

    auto Session::_getFlop(const cv::Mat& screenshot, bool checkHasChanged) -> void {
        _board1CardImg.setCurrentImg(screenshot);
        _board2CardImg.setCurrentImg(screenshot);
        _board3CardImg.setCurrentImg(screenshot);

        if (checkHasChanged && !_board3CardImg.hasChanged(screenshot)) {
            LOG_DEBUG(Logger::getLogger(), "Flop cards have not changed");
            return;
        }

        if (!_board1CardImg.isChecked() || !_board2CardImg.isChecked() || !_board3CardImg.isChecked()) {
            LOG_DEBUG(Logger::getLogger(), "Flop cards are not checked");
            return;
        }

        auto card1 = _ocr->readBoardCard(_board1CardImg.getImg());
        auto card2 = _ocr->readBoardCard(_board2CardImg.getImg());
        auto card3 = _ocr->readBoardCard(_board3CardImg.getImg());

        _game.getCurrentRound().getBoard().setFlop({card1, card2, card3});

        LOG_INFO(Logger::getLogger(), "Flop: {}-{}-{}", card1, card2, card3);
    }

    auto Session::_getTurn(const cv::Mat& screenshot, bool checkHasChanged) -> void {
        _board4CardImg.setCurrentImg(screenshot);

        if (checkHasChanged && !_board4CardImg.hasChanged(screenshot)) {
            LOG_DEBUG(Logger::getLogger(), "Turn card has not changed");
            return;
        }

        if (!_board4CardImg.isChecked()) {
            LOG_DEBUG(Logger::getLogger(), "Turn card is not checked");
            return;
        }

        auto card = _ocr->readBoardCard(_board4CardImg.getImg());

        _game.getCurrentRound().getBoard().setTurn(card);

        LOG_INFO(Logger::getLogger(), "Turn: {}", card);
    }

    auto Session::_getRiver(const cv::Mat& screenshot, bool checkHasChanged) -> void {
        _board5CardImg.setCurrentImg(screenshot);

        if (checkHasChanged && !_board4CardImg.hasChanged(screenshot)) {
            LOG_DEBUG(Logger::getLogger(), "River card has not changed");
            return;
        }

        if (!_board5CardImg.isChecked()) {
            LOG_DEBUG(Logger::getLogger(), "River card is not checked");
            return;
        }

        writeLogGameImage(_board5CardImg.getImg(), LOGS_DIR, "river_card");
        auto card = _ocr->readBoardCard(_board5CardImg.getImg());

        _game.getCurrentRound().getBoard().setRiver(card);

        LOG_INFO(Logger::getLogger(), "River: {}", card);
    }

    auto Session::_getStreetCards(const cv::Mat& screenshot, const Round& round) -> void {
        try {
            // @todo add a _flopVerified bool flag to check cards twice
            if (round.getCurrentStreet() >= FLOP && round.getBoard().isFlopEmpty()) { _getFlop(screenshot); }
            if (round.getCurrentStreet() >= TURN && round.getBoard().getTurn().isUnknown()) { _getTurn(screenshot); }
            if (round.getCurrentStreet() >= RIVER && round.getBoard().getRiver().isUnknown()) { _getRiver(screenshot); }
        } catch (const CannotReadBoardCardImageException& e) {
            LOG_DEBUG(Logger::getLogger(), "Cannot read {} card", round.getCurrentStreet());
        }
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
            // Get player names and initialize the game
            _game.init(_ocr->readPlayerName(_scraper.getPlayer1NameImg(screenshot)),
                       _ocr->readPlayerName(_scraper.getPlayer2NameImg(screenshot)),
                       _ocr->readPlayerName(_scraper.getPlayer3NameImg(screenshot)));
            // End the initialization, wait for the first round to start
            LOG_INFO(Logger::getLogger(), "{}", _game);

            _gameStage = WAITING_NEW_ROUND;
        } catch (const ExceptionWithImage& e) {
            LOG_DEBUG(Logger::getLogger(), "{}", e.what());

            writeLogGameImage(e.getImage(), LOGS_DIR, e.getCategory());
        }
    }

    auto Session::_isNextActionTriggered(const cv::Mat& screenshot) -> bool {
        auto currentActionImg      = _scraper.getPlayerActionImg(screenshot, _currentPlayerNum);
        bool isNextActionTriggered = !isSimilar(currentActionImg, _lastWaitingActionImg, ACTION_SIMILARITY_THRESHOLD);

        _lastWaitingActionImg = currentActionImg;

        return isNextActionTriggered;
    }

    auto Session::_trackCurrentRound(const cv::Mat& screenshot) -> void {
        try {
            auto& round = _game.getCurrentRound();
            // During the showdown process we must check if the round is over, @todo refactor this
            if (round.isInProgress()) {
                _playerActionImg.setCurrentImg(screenshot, _currentPlayerNum);

                if (_playerActionImg.isChecked(_currentPlayerNum)) {
                    _determinePlayerAction(screenshot, _playerActionImg.getImg(_currentPlayerNum), _currentPlayerNum);
                }
            }
            // If the round is over, determine if the game is over or wait for a new round
            if (!round.isInProgress()) {
                LOG_DEBUG(Logger::getLogger(), "Round recap:\n{}", round.toJson().dump(4));

                _determineGameOver();

                if (!_game.isOver()) { _gameStage = WAITING_NEW_ROUND; }
            }
        } catch (const CannotReadGameActionImageException& e) {
            // Should be CannotReadGameActionImageException or CannotReadPlayerBetImageException
            LOG_ERROR(Logger::getLogger(), "{}", e.what());

            writeLogPlayerImage(e.getImage(), LOGS_DIR, e.getCategory(), _currentPlayerNum);
        }
    }

    auto Session::_waitGameStart(const cv::Mat& screenshot) -> void {
        try {
            // Get hand
            auto firstCard  = _ocr->readPlayerCard(_scraper.getFirstCardImg(screenshot));
            auto secondCard = _ocr->readPlayerCard(_scraper.getSecondCardImg(screenshot));
            // Get button position
            _getButtonPosition(screenshot);
            // If both hand and button position are valid, the game has started
            LOG_INFO(Logger::getLogger(), "Game started");

            _gameStage = GAME_INFO_SETUP;
        } catch (const CannotFindButtonException& e) {
            LOG_DEBUG(Logger::getLogger(), "Waiting game start, button not found");
        } catch (const CannotReadPlayerCardImageException& e) {
            LOG_DEBUG(Logger::getLogger(), "Waiting game start, hand not distributed");
        }
    }

    auto Session::_waitNewRound(const cv::Mat& screenshot) -> void {
        try {
            // Get hand
            auto firstCard  = _ocr->readPlayerCard(_scraper.getFirstCardImg(screenshot));
            auto secondCard = _ocr->readPlayerCard(_scraper.getSecondCardImg(screenshot));
            Hand hand       = {firstCard, secondCard};
            // Get round blind level
            auto blinds = _ocr->readBlindRange(_scraper.getBlindAmountImg(screenshot));
            // Get button position
            auto buttonPosition = _getButtonPosition(screenshot);
            // If the button position did not change, the new round did not start
            if (buttonPosition == _currentButtonNum) {
                LOG_DEBUG(Logger::getLogger(), "Waiting new round, button did not move");

                return;
            }
            // Update current button and current player position and reset showdown flag
            _currentButtonNum     = buttonPosition;
            _currentPlayerNum     = buttonPosition;
            _showdownTriggered    = false;
            _lastWaitingActionImg = _scraper.getPlayerActionImg(screenshot, _currentPlayerNum);
            // Start round
            LOG_INFO(Logger::getLogger(), "New round: [blinds {}] [hand {}] [dealer (player_{})]", blinds, hand, _currentButtonNum);

            _game.newRound(blinds, hand, _currentButtonNum);
            _gameStage = ROUND_IN_PROGRESS;
        } catch (const CannotFindButtonException& e) {
            LOG_DEBUG(Logger::getLogger(), "Waiting new round, cannot find the button");
        } catch (const CannotReadPlayerCardImageException& e) {
            LOG_DEBUG(Logger::getLogger(), "Waiting new round, hand not distributed");
        }
    }

    auto Session::_setShowdownComparisonCards(const cv::Mat& screenshot) -> void {
        _board1CardImg.setComparisonImg(screenshot, _currentPlayerNum);
        _board2CardImg.setComparisonImg(screenshot, _currentPlayerNum);
        _board3CardImg.setComparisonImg(screenshot, _currentPlayerNum);
        _board4CardImg.setComparisonImg(screenshot, _currentPlayerNum);
        _board5CardImg.setComparisonImg(screenshot, _currentPlayerNum);
    }

    auto Session::_waitShowdown(const cv::Mat& screenshot) -> void {
        try {
            LOG_DEBUG(Logger::getLogger(), "waitShowdown");
            auto&       round = _game.getCurrentRound();
            const auto& board = round.getBoard();
            // Wait board cards
            if (board.isFlopEmpty()) { return _getFlop(screenshot, true); }
            if (board.getTurn().isUnknown()) { return _getTurn(screenshot, true); }
            if (board.getRiver().isUnknown()) { return _getRiver(screenshot, true); }
            // Get player hands
            for (auto playerNum : _game.getCurrentRound().getInRoundPlayersNum()) {
                if (playerNum == 1) { continue; }  // Skip player 1, as it is the bot
                if (!round.getPlayerHand(playerNum).isSet()) {
                    try {
                        Hand hand;
                        // Cards can be a bit lowered when the hand loses, so we try to read both positions
                        // @todo refactor this, make a getPlayerHandLowerImg with a height parameter
                        try {
                            hand = _ocr->readHand(_scraper.getPlayerHandImg(screenshot, playerNum));
                        } catch (const CannotReadPlayerCardImageException& e) {
                            hand = _ocr->readHand(_scraper.getPlayerCardsImg(screenshot, playerNum));
                        }

                        round.setPlayerHand(hand, playerNum);
                        // @todo verify all board cards (could be misread on certain frames)

                        LOG_INFO(Logger::getLogger(), "player {} hand: {}-{}", playerNum, hand.getCards()[0], hand.getCards()[1]);
                    } catch (const CannotReadPlayerCardImageException& e) {
                        LOG_DEBUG(Logger::getLogger(), "{}. Player {}", e.what(), playerNum);

                        writeLogPlayerImage(e.getImage(), LOGS_DIR, e.getCategory(), playerNum);

                        throw;
                    }
                }
            }

            round.showdown();
        } catch (const CannotReadPlayerCardImageException& e) { LOG_DEBUG(Logger::getLogger(), "Waiting showdown"); }
    }
}  // namespace GameSession