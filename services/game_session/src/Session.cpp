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
            _game      = std::move(other._game);
            _scraper   = std::move(other._scraper);
            _tickRate  = other._tickRate;
            _windowId  = other._windowId;
            _gamePhase = other._gamePhase;
        }

        return *this;
    }

    auto Session::run() -> void {
        _currentScreenshot = _scraper.getScreenshot(_windowId);

        _harvestGameInfo(_currentScreenshot);

        while (_gamePhase != GamePhases::ENDED)
        {
            switch (_gamePhase)
            {
                case GamePhases::STARTING:
                    if (!_game.isInitialized()) { _harvestGameInfo(_currentScreenshot); }
                    break;
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

            _currentScreenshot = _scraper.getScreenshot(_windowId);
        }
    }

    auto Session::_determineGameEvent() -> GameEvent { return PLAYER_ACTION; }

    auto Session::_harvestGameInfo(const cv::Mat& screenshot) -> void {
        try
        {
            // Get player names
            auto player1NameImg = _scraper.getPlayer1NameImg(screenshot);
            auto player2NameImg = _scraper.getPlayer2NameImg(screenshot);
            auto player3NameImg = _scraper.getPlayer3NameImg(screenshot);

            _game.init(_ocr->readWord(player1NameImg), _ocr->readWord(player2NameImg), _ocr->readWord(player3NameImg));

            // Get prize pool
            auto prizePoolImg = _scraper.getPrizePoolImg(screenshot);

            _game.setMultipliers(_ocr->readIntNumbers(prizePoolImg) / _game.getBuyIn());

            // Get player stacks
            auto player1StackImg = _scraper.getPlayer1StackImg(screenshot);
            auto player2StackImg = _scraper.getPlayer2StackImg(screenshot);
            auto player3StackImg = _scraper.getPlayer3StackImg(screenshot);

            _game.getPlayer1().setStack(_ocr->readIntNumbers(player1StackImg));
            _game.getPlayer2().setStack(_ocr->readIntNumbers(player2StackImg));
            _game.getPlayer3().setStack(_ocr->readIntNumbers(player3StackImg));

            // Get button position
            auto player1ButtonImg = _scraper.getPlayer1ButtonImg(screenshot);
            auto player2ButtonImg = _scraper.getPlayer2ButtonImg(screenshot);
            auto player3ButtonImg = _scraper.getPlayer3ButtonImg(screenshot);

            if (_ocr->isSimilar(player1ButtonImg, _ocr->getButtonImg()))
            {
                _game.setButton(_game.getPlayer1());
            } else if (_ocr->isSimilar(player2ButtonImg, _ocr->getButtonImg())) {
                _game.setButton(_game.getPlayer2());
            } else if (_ocr->isSimilar(player3ButtonImg, _ocr->getButtonImg())) {
                _game.setButton(_game.getPlayer3());
            } else {
                LOG_ERROR(Logger::getLogger(), "Could not determine button position");
            }

            // Get round pot
            auto potImg = _scraper.getPotImg(screenshot);

            _game.getCurrentRound().setPot(_ocr->readIntNumbers(potImg));
        } catch (const invalid_player_name& error)
        { LOG_INFO(Logger::getLogger(), "{}", error.what()); }
    }

    auto Session::_evaluatePlayerAction() -> RoundAction {
        RoundAction action(CHECK, _game.getPlayer1(), seconds(1));
        return action;
    }

    auto Session::_processPlayerAction(const RoundAction& action) -> void {}
}  // namespace GameSession