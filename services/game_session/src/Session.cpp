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
        while (_gameStage != ENDED) {
            _lastTick          = now();
            _currentScreenshot = _getScreenshot();
            auto& round        = _game.getCurrentRound();

            DISPLAY_VIDEO("game", _scraper.getWindowElementsView(*_currentScreenshot));

            switch (_gameStage) {
                case STARTING: _waitGameStart(*_currentScreenshot); break;
                case GAME_INFO_SETUP: _harvestGameInfo(*_currentScreenshot); break;
                case WAITING_NEW_ROUND: _waitNewRound(*_currentScreenshot); break;
                case ROUND_IN_PROGRESS:
                    if (round.waitingShowdown()) {
                        _waitShowdown(*_currentScreenshot);
                    } else {
                        _getStreetCards(*_currentScreenshot, round);  // Always get sure to get the street cards

                        if (_currentAction != NONE || _isNextActionTriggered(*_currentScreenshot)) {
                            _trackCurrentRound(*_currentScreenshot);
                        }
                    }

                    break;
                case ENDED: break;
            }

            std::this_thread::sleep_for(_tickRate - (now() - _lastTick));
        }
    }

    // Used for testing by mocking this method
    auto Session::_getScreenshot() -> sharedConstMat_t { return _scraper.getScreenshot(_windowId); }

    auto Session::_determineGameOver() -> void {
        const auto& players = _game.getPlayers();

        auto winnerFinder = [](const Player& player) { return !player.isEliminated(); };
        // Game is over if player 1 is eliminated or if there is only one player left
        if (_game.getPlayer(1).isEliminated() || std::count_if(players.begin(), players.end(), winnerFinder) == 1) {
            _game.end();
            _gameStage = ENDED;
        }
    }

    auto Session::_processPlayerAction(const cv::Mat& screenshot, const cv::Mat& actionImg, int32_t playerNum) -> void {
        auto& round = _game.getCurrentRound();

        // readPlayerBet may fail, so we keep the action until we can read the bet and reset the action to NONE in case of success
        if (_currentAction == NONE) { _currentAction = _readPlayerAction(screenshot, actionImg, playerNum); }

        if (_currentAction == BET || _currentAction == RAISE) {
            _playerBetImg.setCurrentImg(screenshot, playerNum);

            if (!_playerBetImg.isChecked(playerNum)) { return; }
        }

        switch (_currentAction) {
            case FOLD: round.fold(playerNum); break;
            case CHECK: round.check(playerNum); break;
            case CALL: round.call(playerNum); break;
            case ALL_IN: round.allIn(playerNum); break;
            case BET: round.bet(playerNum, _ocr->readPlayerBet(_playerBetImg.getImg(playerNum))); break;
            case RAISE: round.raiseTo(playerNum, _ocr->readPlayerBet(_playerBetImg.getImg(playerNum))); break;
            case PAY_BIG_BLIND:
            case PAY_SMALL_BLIND: _currentAction = NONE; return;
            case NONE: throw CannotReadGameActionImageException(actionImg, "NONE");
        }

        LOG_INFO(Logger::getLogger(), "{}", round.getLastAction());

        _currentPlayerNum     = round.getCurrentPlayerNum();
        _lastWaitingActionImg = _scraper.getPlayerActionImg(screenshot, _currentPlayerNum);
        _currentAction        = NONE;

        if (!round.isInProgress()) { _endRound(); }
    }

    auto Session::_readPlayerAction(const cv::Mat& screenshot, const cv::Mat& actionImg, int32_t playerNum) -> ActionType {
        ActionType currentAction;

        if (_ocr->isAllIn(_scraper.getPlayerStackImg(screenshot, playerNum))) {
            currentAction = ALL_IN;
        } else {
            try {
                currentAction = _ocr->readGameAction(actionImg);
            } catch (const CannotReadGameActionImageException& e) {
                auto action = _readPlayerActionFallback(screenshot, playerNum);

                if (action != NONE) {
                    currentAction = action;
                } else {
                    throw;
                }
            }
        }

        return currentAction;
    }

    auto Session::_readPlayerActionFallback(const cv::Mat& screenshot, int32_t playerNum) -> ActionType {
        ActionType action = NONE;
        auto&      round  = _game.getCurrentRound();

        // Fallback only when the current player is the last player to act and other players are all-in
        if (round.isNextActionTheLastStreetOne(playerNum)) {
            LOG_DEBUG(Logger::getLogger(), "Try fallback methods to read action");

            auto playerStack = _ocr->readPlayerStack(_scraper.getPlayerStackImg(screenshot, playerNum));

            if (playerStack != round.getCurrentPlayerStack(playerNum)) {
                LOG_DEBUG(Logger::getLogger(), "Player {} stack has changed, action is CALL", playerNum);

                action = CALL;
            } else {
                LOG_DEBUG(Logger::getLogger(), "Player {} stack has not changed, action is FOLD", playerNum);

                action = FOLD;
            }
        }

        return action;
    }

    auto Session::_getButtonPosition(const cv::Mat& screenshot) -> int32_t {
        for (int32_t i = 1; i <= 3; i++) {
            if (!isSimilar(_scraper.getPlayerButtonImg(screenshot, i), _ocr->getButtonImg())) { continue; }

            return i;
        }

        throw CannotFindButtonException();
    }

    auto Session::_getFlop(const cv::Mat& screenshot) -> void {
        _board1CardImg.setCurrentImg(screenshot);
        _board2CardImg.setCurrentImg(screenshot);
        _board3CardImg.setCurrentImg(screenshot);

        if (!_board3CardImg.hasChanged(screenshot) || !_board1CardImg.isChecked() || !_board2CardImg.isChecked()
            || !_board3CardImg.isChecked()) {
            return;
        }

        auto card1 = _ocr->readBoardCard(_board1CardImg.getImg());
        auto card2 = _ocr->readBoardCard(_board2CardImg.getImg());
        auto card3 = _ocr->readBoardCard(_board3CardImg.getImg());

        _game.getCurrentRound().getBoard().setFlop({card1, card2, card3});

        LOG_INFO(Logger::getLogger(), "Flop: {}-{}-{}", card1, card2, card3);
    }

    auto Session::_getTurn(const cv::Mat& screenshot) -> void {
        _board4CardImg.setCurrentImg(screenshot);

        if (!_board4CardImg.hasChanged(screenshot) || !_board4CardImg.isChecked()) { return; }

        auto card = _ocr->readBoardCard(_board4CardImg.getImg());

        _game.getCurrentRound().getBoard().setTurn(card);

        LOG_INFO(Logger::getLogger(), "Turn: {}", card);
    }

    auto Session::_getRiver(const cv::Mat& screenshot) -> void {
        _board5CardImg.setCurrentImg(screenshot);

        if (!_board5CardImg.hasChanged(screenshot) || !_board5CardImg.isChecked()) { return; }

        auto card = _ocr->readBoardCard(_board5CardImg.getImg());

        _game.getCurrentRound().getBoard().setRiver(card);

        LOG_INFO(Logger::getLogger(), "River: {}", card);
    }

    auto Session::_getStreetCards(const cv::Mat& screenshot, const Round& round) -> void {
        try {
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
        } catch (const ExceptionWithImage& e) { LOG_DEBUG(Logger::getLogger(), "{}", e.what()); }
    }

    auto Session::_isNextActionTriggered(const cv::Mat& screenshot) -> bool {
        _playerActionImg.setCurrentImg(screenshot, _currentPlayerNum);
        // If the player action image has not been set yet, we use the comparison image to determine if the action has changed
        if (_playerActionImg.getComparisonImg(_currentPlayerNum).empty()) {
            auto currentActionImg = _scraper.getPlayerActionImg(screenshot, _currentPlayerNum);

            bool isNextActionTriggered = !isSimilar(currentActionImg, _lastWaitingActionImg, ACTION_SIMILARITY_THRESHOLD);

            _lastWaitingActionImg = currentActionImg;

            return isNextActionTriggered;
        } else {
            if (!_playerActionImg.hasChanged(screenshot, _currentPlayerNum)) { return false; }

            if (!_playerActionImg.isChecked(_currentPlayerNum)) {
                LOG_DEBUG(Logger::getLogger(), "Player action is not checked");
                return false;
            }

            return true;
        }
    }

    auto Session::_trackCurrentRound(const cv::Mat& screenshot) -> void {
        try {
            //_playerActionImg.setCurrentImg(screenshot, _currentPlayerNum);
            _processPlayerAction(screenshot, _playerActionImg.getImg(_currentPlayerNum), _currentPlayerNum);
        } catch (const ExceptionWithImage& e) {
            // Should be CannotReadGameActionImageException or CannotReadPlayerBetImageException or CannotReadPlayerStackImageException
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
            auto& round = _game.getCurrentRound();
            // Get button position
            auto buttonPosition = _getButtonPosition(screenshot);

            if (!_game.hasNoRound() && round.playerGotBusted()) {
                // If self stack did not change, the blinds are not paid yet so the new round did not start yet
                if (_ocr->readPlayerStack(_scraper.getPlayerStackImg(screenshot, 1)) == round.getCurrentPlayerStack(1)) {
                    LOG_DEBUG(Logger::getLogger(), "Waiting new round, player got busted and stacks did not change");

                    return;
                }
            } else if (buttonPosition == _currentButtonNum) {
                // If the button position did not change, the new round did not start yet
                LOG_DEBUG(Logger::getLogger(), "Waiting new round, button did not move");

                return;
            }
            // Get hand
            auto firstCard  = _ocr->readPlayerCard(_scraper.getFirstCardImg(screenshot));
            auto secondCard = _ocr->readPlayerCard(_scraper.getSecondCardImg(screenshot));
            Hand hand       = {firstCard, secondCard};
            // Get round blind level
            auto blinds = _ocr->readBlindRange(_scraper.getBlindAmountImg(screenshot));
            // Update current button and current player position and reset showdown flag
            _currentButtonNum     = buttonPosition;
            _currentPlayerNum     = buttonPosition;
            _showdownTriggered    = false;
            _lastWaitingActionImg = _scraper.getPlayerActionImg(screenshot, _currentPlayerNum);
            // Get player empty action comparison image if not already set
            if (_playerActionImg.getComparisonImg(_currentPlayerNum).empty()) {
                _playerActionImg.setComparisonImg(screenshot, _currentPlayerNum);
            }
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
            if (board.isFlopEmpty()) { return _getFlop(screenshot); }
            if (board.getTurn().isUnknown()) { return _getTurn(screenshot); }
            if (board.getRiver().isUnknown()) { return _getRiver(screenshot); }
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

                        LOG_INFO(Logger::getLogger(), "player {} hand: {}-{}", playerNum, hand.getCards()[0], hand.getCards()[1]);
                    } catch (const CannotReadPlayerCardImageException& e) {
                        LOG_DEBUG(Logger::getLogger(), "{}. Player {}", e.what(), playerNum);

                        writeLogPlayerImage(e.getImage(), LOGS_DIR, e.getCategory(), playerNum);

                        throw;
                    }
                }
            }
            // End the round
            round.showdown();
            _endRound();
        } catch (const CannotReadPlayerCardImageException& e) {
            LOG_DEBUG(Logger::getLogger(), "Waiting player cards");
        } catch (const CannotReadBoardCardImageException& e) { LOG_DEBUG(Logger::getLogger(), "Waiting board cards"); }
    }

    auto Session::_endRound() -> void {
        LOG_DEBUG(Logger::getLogger(), "Round recap:\n{}", _game.getCurrentRound().toJson().dump(4));

        _determineGameOver();

        if (!_game.isOver()) { _gameStage = WAITING_NEW_ROUND; }
    }
}  // namespace GameSession