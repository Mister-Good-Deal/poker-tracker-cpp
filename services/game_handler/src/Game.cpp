#include "Game.hpp"

namespace GameHandler {
    using std::ranges::for_each;
    using std::views::keys;

    auto Game::operator=(Game&& other) noexcept -> Game& {
        if (this != &other)
        {
            _rounds      = std::move(other._rounds);
            _players     = std::move(other._players);
            _startTime   = std::move(other._startTime);
            _endTime     = std::move(other._endTime);
            _winner      = other._winner;
            _buyIn       = other._buyIn;
            _multipliers = other._multipliers;
        }

        return *this;
    }

    auto Game::operator=(const Game& other) -> Game& {
        if (this != &other)
        {
            _rounds      = other._rounds;
            _players     = other._players;
            _startTime   = other._startTime;
            _endTime     = other._endTime;
            _winner      = other._winner;
            _buyIn       = other._buyIn;
            _multipliers = other._multipliers;
        }

        return *this;
    }

    auto Game::init(const Player& player1, const Player& player2, const Player& player3) -> void {
        _players[player1.getName()] = player1;
        _players[player2.getName()] = player2;
        _players[player3.getName()] = player3;

        newRound();

        _startTime = system_clock::now();
    }

    auto Game::end() -> void { _endTime = system_clock::now(); }
    auto Game::newRound() -> void { _rounds.emplace_back(); }

    auto Game::getPlayer(const std::string& playerName) -> Player {
        if (_players.find(playerName) == _players.end())
        { throw UnknownPlayerException("The player " + playerName + " is unknown in this game"); }

        return _players.at(playerName);
    }

    auto Game::toJson() const -> json {
        if (_winner == nullptr) { throw std::runtime_error("The game's winner has not been set"); }

        auto roundsArray      = json::array();
        auto playersNameArray = json::array();

        for_each(_rounds, [&roundsArray](const Round& round) { roundsArray.emplace_back(round.toJson()); });
        for_each(_players | keys, [&playersNameArray](const auto& playerName) { playersNameArray.emplace_back(playerName); });

        return {{"rounds", roundsArray},
                {"players", playersNameArray},
                {"winner", _winner->getName()},
                {"won", _winner->self()},
                {"buy_in", _buyIn},
                {"multipliers", _multipliers},
                {"balance", _computeBalance()},
                {"duration", duration_cast<seconds>(_endTime - _startTime).count()}};
    }

    auto Game::_computeBalance() const -> int32_t { return _buyIn * ((_winner->self() ? _multipliers : 0) - 1); }

}  // namespace GameHandler