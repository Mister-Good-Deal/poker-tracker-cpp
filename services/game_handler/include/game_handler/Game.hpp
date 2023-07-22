#pragma once

#include <array>

#include <game_handler/Round.hpp>

namespace GameHandler {
    using std::chrono::system_clock;
    using std::chrono::time_point;

    class invalid_player_name : public std::runtime_error {
        public:
            explicit invalid_player_name(const std::string& arg) : runtime_error(arg){};
    };

    class Game {
        public:
            Game()                  = default;
            Game(const Game& other) = default;
            Game(Game&& other) noexcept { *this = std::move(other); };

            virtual ~Game() = default;

            auto operator=(const Game& other) -> Game& = default;
            auto operator=(Game&& other) noexcept -> Game&;

            auto getCurrentRound() -> Round& { return _rounds.back(); };
            auto getPlayer1() -> Player& { return _players[0]; };
            auto getPlayer2() -> Player& { return _players[1]; };
            auto getPlayer3() -> Player& { return _players[2]; };
            auto getPlayer(uint8_t playerNum) -> Player&;
            auto getPlayer(const std::string& playerName) -> Player&;
            auto getPlayers() -> std::array<Player, 3>& { return _players; };

            [[nodiscard]] auto isInitialized() const -> bool { return _initialized; };
            [[nodiscard]] auto getBuyIn() const -> int32_t { return _buyIn; };

            auto setBuyIn(int32_t buyIn) -> void { _buyIn = buyIn; }
            auto setMultipliers(int32_t multipliers) -> void { _multipliers = multipliers; }
            auto setWinner(const Player& winner) -> void { _winner = &winner; }
            auto setCurrentPlaying(const Player& player) -> void { _currentPlaying = &player; }
            auto setComplete(bool complete) -> void { _complete = complete; }

            auto init(const std::string& player1Name, const std::string& player2Name, const std::string& player3Name) -> void;
            auto newRound() -> void;
            auto end() -> void;

            [[nodiscard]] auto toJson() const -> json;

        private:
            std::vector<Round>       _rounds;
            std::array<Player, 3>    _players;
            time_point<system_clock> _startTime;
            time_point<system_clock> _endTime;
            const Player*            _winner         = nullptr;
            const Player*            _currentPlaying = nullptr;
            int32_t                  _buyIn          = 0;
            int32_t                  _multipliers    = 2;
            bool                     _initialized    = false;
            bool                     _complete       = true;

            [[nodiscard]] auto _computeBalance() const -> int32_t;
    };
}  // namespace GameHandler