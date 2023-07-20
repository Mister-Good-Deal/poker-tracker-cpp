#include "game_handler/Game.hpp"

namespace GameHandler {
    using std::ranges::any_of;
    using std::ranges::for_each;

    auto Game::operator=(Game&& other) noexcept -> Game& {
        if (this != &other)
        {
            _rounds      = std::move(other._rounds);
            _players     = std::move(other._players);
            _startTime   = other._startTime;
            _endTime     = other._endTime;
            _winner      = other._winner;
            _buyIn       = other._buyIn;
            _multipliers = other._multipliers;
        }

        return *this;
    }

    auto Game::init(const std::string& player1Name, const std::string& player2Name, const std::string& player3Name) -> void {
        auto nameList = {player1Name, player2Name, player3Name};

        if (any_of(nameList, [](const auto& name) { return name.empty(); }))
        {
            throw invalid_player_name(fmt::format("A player's name is empty, player_1 `{}`, player_2 `{}`, player_3 `{}`", player1Name,
                                                  player2Name, player3Name));
        }

        _players[0] = Player(player1Name, true);
        _players[1] = Player(player2Name);
        _players[2] = Player(player3Name);

        newRound();

        _startTime   = system_clock::now();
        _initialized = true;
    }

    auto Game::end() -> void { _endTime = system_clock::now(); }
    auto Game::newRound() -> void { _rounds.emplace_back(); }

    auto Game::getPlayer(const std::string& playerName) -> Player& {
        for (auto& player : _players)
        {
            if (player.getName() == playerName) { return player; }
        }

        throw UnknownPlayerException("The player " + playerName + " is unknown in this game");
    }

    auto Game::toJson() const -> json {
        if (_winner == nullptr) { throw std::runtime_error("The game's winner has not been set"); }

        auto roundsArray      = json::array();
        auto playersNameArray = json::array();

        for_each(_rounds, [&roundsArray](const Round& round) { roundsArray.emplace_back(round.toJson()); });
        for_each(_players, [&playersNameArray](const Player& player) { playersNameArray.emplace_back(player.getName()); });

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