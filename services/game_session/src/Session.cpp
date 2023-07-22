#include "game_session/Session.hpp"

#include <logger/Logger.hpp>

namespace GameSession {
    using GameHandler::invalid_player_name;

    using Logger = Logger::Quill;

    using enum RoundAction::ActionType;

    Session::Session(std::string_view roomName, uint64_t windowId) :
        _roomName(roomName), _windowId(windowId), _scraper(_roomName, {0, 0}), _ocr(OcrFactory::create(_roomName)), _game() {}

    auto Session::operator=(Session&& other) noexcept -> Session& {
        if (this != &other)
        {
            _game              = std::move(other._game);
            _scraper           = std::move(other._scraper);
            _tickRate          = other._tickRate;
            _windowId          = other._windowId;
            _gameStage         = other._gameStage;
            _currentPlaying    = other._currentPlaying;
            _currentPlayingNum = other._currentPlayingNum;
        }

        return *this;
    }

    auto Session::run() -> void {
        _currentScreenshot = _scraper.getScreenshot(_windowId);

        _harvestGameInfo(_currentScreenshot);

        while (_gameStage != GameStages::ENDED)
        {
            switch (_gameStage)
            {
                case GameStages::STARTING:
                    if (!_game.isInitialized()) { _harvestGameInfo(_currentScreenshot); }

                    break;
                case GameStages::IN_PROGRESS:
                    while (_game.getCurrentRound().isInProgress())
                    {
                        _trackCurrentRound(_currentScreenshot);
                        // _game.getCurrentRound().end();
                    }

                    if (_isGameOver())
                    {
                        _game.end();
                        _gameStage = GameStages::ENDED;
                    } else {
                        _game.newRound();
                    }

                    break;
                case GameStages::ENDED: break;
            }

            std::this_thread::sleep_for(_tickRate);

            _currentScreenshot = _scraper.getScreenshot(_windowId);
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
        try
        {
            // Get player names
            _game.init(_ocr->readPlayerName(_scraper.getPlayer1NameImg(screenshot)),
                       _ocr->readPlayerName(_scraper.getPlayer2NameImg(screenshot)),
                       _ocr->readPlayerName(_scraper.getPlayer3NameImg(screenshot)));

            // Determine if game was already started
            // @todo use game duration to determine if game was already started
            if (_ocr->readBlindLevel(_scraper.getBlindLevelImg(screenshot)) > 1) { _game.setComplete(false); }

            // Get prize pool
            _game.setMultipliers(_ocr->readPrizePool(_scraper.getPrizePoolImg(screenshot)) / _game.getBuyIn());
        } catch (const invalid_player_name& error)
        { LOG_INFO(Logger::getLogger(), "{}", error.what()); }
    }

    auto Session::_initCurrentRound(const cv::Mat& screenshot) -> void {
        // Get round pot
        auto pot = _ocr->readPot(_scraper.getPotImg(screenshot));
        // Wait for the pot to be initialized with ante or blinds
        if (pot == 0) { throw PotNotInitializedException("Pot is not initialized"); }
        // Get player stacks
        for (uint8_t i = 1; i <= 3; i++)
        { _game.getPlayer(i).setStack(_ocr->readIntNumbers(_scraper.getPlayerStackImg(screenshot, i))); }
        // Get button position
        _assignButton(screenshot);
        // Get round blind level
        auto blinds = _ocr->readBlindRange(_scraper.getBlindAmountImg(screenshot));
        // Get hand
        auto firstCard  = _ocr->readCard(_scraper.getFirstCardImg(screenshot));
        auto secondCard = _ocr->readCard(_scraper.getSecondCardImg(screenshot));

        _game.getCurrentRound().init({firstCard, secondCard}, blinds, pot, _game.getPlayer1(), _game.getPlayer2(), _game.getPlayer3());
    }

    auto Session::_trackCurrentRound(const cv::Mat& screenshot) -> void {
        try
        {
            if (!_game.getCurrentRound().isInitialized()) { _initCurrentRound(screenshot); }

            // @todo handle loop when player is not playing with last action image comparison

            _determinePlayerAction(screenshot, *_currentPlaying, _currentPlayingNum);
        } catch (const PotNotInitializedException& error)
        { LOG_INFO(Logger::getLogger(), "{}", error.what()); }
    }

    auto Session::_determinePlayerAction(const cv::Mat& screenshot, const Player& player, uint8_t playerNum) -> void {
        if (playerNum != 2 && playerNum != 3)
        { throw WrongCurrentPlayingPlayerException("Current playing player must be either player 2 or 3"); }

        // Did he fold ?
        if (_ocr->hasFolded(_scraper.getPlayerCardsImg(screenshot, playerNum)))
        {
            _game.getCurrentRound().fold(player);
        } else {
            auto action = _ocr->readGameAction(_scraper.getPlayerActionImg(screenshot, playerNum));

            // Did he check ?
            if (action == "CHECK")
            {
                _game.getCurrentRound().check(player);
            } else {
                auto betAmount = _ocr->readPlayerBet(_scraper.getPlayerBetImg(screenshot, playerNum));

                // Did he bet or call ?
                if (betAmount == _game.getCurrentRound().getLastBet())
                {
                    _game.getCurrentRound().call(player, betAmount);
                } else {
                    _game.getCurrentRound().bet(player, betAmount);
                }
            }
        }
    }

    auto Session::_isGameOver() -> bool {
        auto players = _game.getPlayers();

        return std::count_if(players.begin(), players.end(), [](const Player& player) { return player.isEliminated(); }) == 2;
    }

    auto Session::_assignButton(const cv::Mat& screenshot) -> void {
        for (uint8_t i = 1; i <= 3; i++)
        {
            if (!_ocr->isSimilar(_scraper.getPlayerButtonImg(screenshot, i), _ocr->getButtonImg())) { continue; }

            _game.getPlayer(i).takeButton();
            _currentPlaying    = &_game.getPlayer(i);
            _currentPlayingNum = i;

            return;
        }

        LOG_ERROR(Logger::getLogger(), "Could not determine button position");
    }
}  // namespace GameSession