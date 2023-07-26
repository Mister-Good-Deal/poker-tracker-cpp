#include "game_handler/Game.hpp"

namespace GameHandler {
    using std::ranges::any_of;
    using std::ranges::for_each;

    auto Game::operator=(Game&& other) noexcept -> Game& {
        if (this != &other)
        {
            _rounds         = std::move(other._rounds);
            _players        = std::move(other._players);
            _startTime      = other._startTime;
            _endTime        = other._endTime;
            _winner         = other._winner;
            _currentPlaying = other._currentPlaying;
            _buyIn          = other._buyIn;
            _multipliers    = other._multipliers;
            _initialized    = other._initialized;
            _complete       = other._complete;
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

        _players[0]  = Player(player1Name, 1);
        _players[1]  = Player(player2Name, 2);
        _players[2]  = Player(player3Name, 3);
        _startTime   = system_clock::now();
        _initialized = true;
    }

    auto Game::end() -> void {
        if (_winner == nullptr) { throw std::runtime_error("The game's winner has not been set"); }

        _endTime = system_clock::now();
        _ended   = true;
    }

    auto Game::newRound(const Hand& hand, const Blinds& blinds) -> void {
        _rounds.emplace_back(hand, blinds, _players);

        getCurrentRound().init(hand, blinds, _buyIn * 3, _players);
    }

    auto Game::getPlayer(uint8_t playerNum) -> Player& {
        if (playerNum <= 0 || playerNum > 3) { throw std::invalid_argument("The given player number is invalid"); }

        return _players.at(playerNum - 1);
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
                {"duration", duration_cast<seconds>(_endTime - _startTime).count()},
                {"complete", _complete}};
    }

    auto Game::_computeBalance() const -> int32_t { return _buyIn * ((_winner->self() ? _multipliers : 0) - 1); }

}  // namespace GameHandler