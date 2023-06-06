#include "Session.hpp"

#include <Logger.hpp>

namespace GameSession {
    using GameHandler::invalid_player_name;

    using Logger = Logger::Quill;

    Session::Session(std::string_view roomName, uint64_t windowId) :
        _roomName(roomName), _windowId(windowId), _scraper(ScraperFactory::create(_roomName)), _ocr(OcrFactory::create(_roomName)),
        _game(), _lastScreenshot(_scraper->getScreenshot(_windowId)) {}

    auto Session::operator=(Session&& other) noexcept -> Session& {
        if (this != &other)
        {
            _game     = std::move(other._game);
            _scraper  = std::move(other._scraper);
            _tickRate = other._tickRate;
            _windowId = other._windowId;
        }

        return *this;
    }

    auto Session::run() -> void {
        while (_gamePhase != GamePhases::ENDED)
        {
            _evaluateGamePhase();

            switch (_gamePhase)
            {
                case GamePhases::STARTING: break;
                case GamePhases::IN_PROGRESS:
                    switch (_determineGameEvent())
                    {
                        case GameEvent::PLAYER_ACTION: _processPlayerAction(_evaluatePlayerAction()); break;
                        case GameEvent::GAME_ACTION: break;  // @todo handle game actions separately from player actions ?
                        case GameEvent::NONE: break;
                    }

                    break;
                case GamePhases::ENDED: break;
            }

            std::this_thread::sleep_for(_tickRate);
        }
    }

    auto Session::_determineGameEvent() -> GameEvent { return PLAYER_ACTION; }

    auto Session::_evaluateGamePhase() -> void {
        _currentScreenshot = _scraper->getScreenshot(_windowId);

        switch (_gamePhase)
        {
            case GamePhases::STARTING:
                auto player1NameImg = _scraper->getPlayer1NameImg(_currentScreenshot);
                auto player1Name    = _ocr->readWord(player1NameImg);

                if (player1Name != "")
                {
                    // Initialize the game
                    auto player2NameImg = _scraper->getPlayer2NameImg(_currentScreenshot);
                    auto player3NameImg = _scraper->getPlayer3NameImg(_currentScreenshot);

                    _game.init(player1Name, _ocr->readWord(player2NameImg), _ocr->readWord(player3NameImg));

                    auto prizePoolImg = _scraper->getPrizePoolImg(_currentScreenshot);

                    _game.setMultipliers(_ocr->readNumbers(prizePoolImg) / _game.getBuyIn());

                    _gamePhase = GamePhases::IN_PROGRESS;
                }

                break;
        }
    }

    auto Session::_harvestGameInfo() -> void {
        try
        {
            // Get player names
            auto player1NameImg = _scraper->getPlayer1NameImg(_currentScreenshot);
            auto player2NameImg = _scraper->getPlayer2NameImg(_currentScreenshot);
            auto player3NameImg = _scraper->getPlayer3NameImg(_currentScreenshot);

            _game.init(_ocr->readWord(player1NameImg), _ocr->readWord(player2NameImg), _ocr->readWord(player3NameImg));

            // Get prize pool
            auto prizePoolImg = _scraper->getPrizePoolImg(_currentScreenshot);

            _game.setMultipliers(_ocr->readNumbers(prizePoolImg) / _game.getBuyIn());

            // Get player stacks
            auto player1StackImg = _scraper->getPlayer1StackImg(_currentScreenshot);
            auto player2StackImg = _scraper->getPlayer2StackImg(_currentScreenshot);
            auto player3StackImg = _scraper->getPlayer3StackImg(_currentScreenshot);

            _game.getPlayer1().setStack(_ocr->readNumbers(player1StackImg));
            _game.getPlayer2().setStack(_ocr->readNumbers(player2StackImg));
            _game.getPlayer3().setStack(_ocr->readNumbers(player3StackImg));

            // Get button position
            auto player1ButtonImg = _scraper->getPlayer1ButtonImg(_currentScreenshot);
            auto player2ButtonImg = _scraper->getPlayer2ButtonImg(_currentScreenshot);
            auto player3ButtonImg = _scraper->getPlayer3ButtonImg(_currentScreenshot);

            if (_ocr->isSimilar(player1ButtonImg, _ocr->getButtonImg()))
            {
                _game.setButton(_game.getPlayer1());
            } else if (_ocr->isSimilar(player2ButtonImg, _ocr->getButtonImg())) {
                _game.setButton(_game.getPlayer2());
            } else if (_ocr->isSimilar(player3ButtonImg, _ocr->getButtonImg())) {
                _game.setButton(_game.getPlayer3());
            } else {
                throw std::runtime_error("Could not determine button position");
            }

            // Get round pot
            auto potImg = _scraper->getPotImg(_currentScreenshot);

            _game.getCurrentRound().setPot(_ocr->readNumbers(potImg));
        } catch (const invalid_player_name& e)
        { LOG_INFO(Logger::getLogger(), e.what()); }
    }
}  // namespace GameSession