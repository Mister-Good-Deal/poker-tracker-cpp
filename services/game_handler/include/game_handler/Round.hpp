#pragma once

#include <game_handler/Board.hpp>
#include <game_handler/RoundAction.hpp>

namespace GameHandler {
    using std::chrono::system_clock;
    using std::chrono::time_point;

    static const int8_t STREET_NUMBER = 5;

    struct Blinds {
        public:
            int32_t smallBlind = 0;
            int32_t bigBlind   = 0;

            Blinds() = default;
            Blinds(int32_t smallBlind, int32_t bigBlind) : smallBlind(smallBlind), bigBlind(bigBlind) {}

            [[nodiscard]] auto SB() const -> int32_t { return smallBlind; }
            [[nodiscard]] auto BB() const -> int32_t { return bigBlind; }
    };

    class UnknownPlayerException : public std::runtime_error {
        public:
            explicit UnknownPlayerException(const std::string& arg) : runtime_error(arg) {}
    };

    class Round {
        public:
            using round_actions_t = std::array<std::vector<RoundAction>, STREET_NUMBER>;

            enum Street : int8_t { PREFLOP = 0, FLOP, TURN, RIVER, SHOWDOWN };

            Round()                   = default;
            Round(const Round& other) = default;
            Round(Round&& other) noexcept { *this = std::move(other); };

            virtual ~Round() = default;

            auto operator=(const Round& other) -> Round& = default;
            auto operator=(Round&& other) noexcept -> Round&;

            [[nodiscard]] auto getBoard() -> Board& { return _board; }
            [[nodiscard]] auto getPot() const -> int32_t { return _pot; }
            [[nodiscard]] auto isInProgress() const -> bool { return !_ended; }
            [[nodiscard]] auto isInitialized() const -> bool { return _initialized; }

            auto init(const Hand& hand, const Blinds& blinds, int32_t pot, Player& player1, Player& player2, Player& player3) -> void;
            auto setHand(const Hand& hand) -> void { _hand = hand; }
            auto setPot(int32_t pot) -> void { _pot = pot; }
            auto setBlinds(const Blinds& blinds) -> void { _blinds = blinds; }
            auto setWinner(const Player& winner) -> void { _winner = &winner; }

            auto start() -> void;
            auto end() -> void;
            auto endStreet() -> void;

            auto call(const Player& player, int32_t amount) -> void;
            auto bet(const Player& player, int32_t amount) -> void;
            auto check(const Player& player) -> void;
            auto fold(const Player& player) -> void;

            [[nodiscard]] auto toJson() const -> json;

        private:
            round_actions_t          _actions;
            Board                    _board;
            Hand                     _hand;
            Blinds                   _blinds         = Blinds(0, 0);
            int32_t                  _pot            = 0;
            int32_t                  _bet            = 0;
            Street                   _currentStreet  = Street::PREFLOP;
            time_point<system_clock> _lastActionTime = system_clock::now();
            const Player*            _player1        = nullptr;  // Player 1 is the Hero
            const Player*            _player2        = nullptr;  // Player 2 is a Villain
            const Player*            _player3        = nullptr;  // Player 3 is a Villain
            const Player*            _winner         = nullptr;
            bool                     _ended          = false;
            bool                     _initialized    = false;

            auto _getAndResetLastActionTime() -> seconds;
    };
}  // namespace GameHandler
