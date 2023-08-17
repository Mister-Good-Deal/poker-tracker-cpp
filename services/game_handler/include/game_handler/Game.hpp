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
            auto getPlayer(uint32_t playerNum) -> Player&;
            auto getPlayers() -> std::array<Player, 3>& { return _players; };

            [[nodiscard]] auto isInitialized() const -> bool { return _initialized; };
            [[nodiscard]] auto isOver() const -> bool { return _ended; };
            [[nodiscard]] auto getBuyIn() const -> int32_t { return _buyIn; };
            [[nodiscard]] auto getMultipliers() const -> int32_t { return _multipliers; };
            [[nodiscard]] auto getInitialStack() const -> int32_t { return _initialStack; };

            auto setBuyIn(int32_t buyIn) -> void { _buyIn = buyIn; }
            auto setMultipliers(int32_t multipliers) -> void { _multipliers = multipliers; }
            auto setInitialStack(int32_t stack) -> void { _initialStack = stack; };
            auto setComplete(bool complete) -> void { _complete = complete; }

            auto init(std::string_view player1Name, std::string_view player2Name, std::string_view player3Name) -> void;
            auto newRound(const Blinds& blinds, const Hand& hand, int32_t dealerNumber) -> Round&;
            auto end() -> void;

            [[nodiscard]] auto toJson() const -> json;

        private:
            std::vector<Round>       _rounds;
            std::array<Player, 3>    _players;  // Player 1 is the Hero, others are Villains
            time_point<system_clock> _startTime;
            time_point<system_clock> _endTime;
            int32_t                  _buyIn        = 0;
            int32_t                  _multipliers  = 2;
            int32_t                  _initialStack = 0;
            bool                     _initialized  = false;
            bool                     _complete     = true;
            bool                     _ended        = false;

            [[nodiscard]] auto _computeBalance() const -> int32_t;
            [[nodiscard]] auto _isGameWon() const -> bool;
    };
}  // namespace GameHandler