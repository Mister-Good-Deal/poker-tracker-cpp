#include "game_handler/Game.hpp"

namespace GameHandler {
    using std::ranges::any_of;
    using std::ranges::for_each;

    auto Game::operator=(Game&& other) noexcept -> Game& {
        if (this != &other) {
            _rounds       = std::move(other._rounds);
            _players      = std::move(other._players);
            _startTime    = other._startTime;
            _endTime      = other._endTime;
            _buyIn        = other._buyIn;
            _multipliers  = other._multipliers;
            _initialStack = other._initialStack;
            _initialized  = other._initialized;
            _complete     = other._complete;
        }

        return *this;
    }

    auto Game::init(std::string_view player1Name, std::string_view player2Name, std::string_view player3Name) -> void {
        auto nameList = {player1Name, player2Name, player3Name};

        if (any_of(nameList, [](const auto& name) { return name.empty(); })) {
            throw invalid_player_name(fmt::format(
                "A player's name is empty, player_1 `{}`, player_2 `{}`, player_3 `{}`", player1Name, player2Name, player3Name));
        }

        _players[0] = Player(player1Name, 1);
        _players[1] = Player(player2Name, 2);
        _players[2] = Player(player3Name, 3);

        for (auto& player : _players) { player.setStack(_initialStack); }

        _startTime   = system_clock::now();
        _initialized = true;
    }

    auto Game::end() -> void {
        _endTime = system_clock::now();
        _ended   = true;
    }

    auto Game::newRound(const Blinds& blinds, const Hand& hand, int32_t dealerNumber) -> Round& {
        _rounds.emplace_back(blinds, _players, hand, dealerNumber);

        return _rounds.back();
    }

    auto Game::getPlayer(uint32_t playerNum) -> Player& {
        if (playerNum <= 0 || playerNum > 3) { throw std::invalid_argument("The given player number is invalid"); }

        return _players.at(playerNum - 1);
    }

    auto Game::toJson() const -> json {
        auto roundsArray      = json::array();
        auto playersNameArray = json::array();

        for_each(_rounds, [&roundsArray](const Round& round) { roundsArray.emplace_back(round.toJson()); });
        for_each(_players, [&playersNameArray](const Player& player) { playersNameArray.emplace_back(player.getName()); });

        return {{"rounds", roundsArray},
                {"players", playersNameArray},
                {"won", _isGameWon()},
                {"buy_in", _buyIn},
                {"multipliers", _multipliers},
                {"balance", _computeBalance()},
                {"duration", duration_cast<seconds>(_endTime - _startTime).count()},
                {"complete", _complete}};
    }

    auto Game::_computeBalance() const -> int32_t { return _buyIn * ((_isGameWon() ? _multipliers : 0) - 1); }

    auto Game::_isGameWon() const -> bool { return _players[0].getStack() != 0; }
}  // namespace GameHandler