#pragma once

#include <Board.hpp>
#include <RoundAction.hpp>

namespace GameHandler {
    using std::chrono::system_clock;
    using std::chrono::time_point;

    static const int8_t STREET_NUMBER = 5;

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

            auto setHand(const Hand& hand) -> void { _hand = hand; }

            auto start() -> void;
            auto endStreet() -> void;
            auto end(const Player& winner) -> void;

            auto call(const Player& player, int32_t amount) -> void;
            auto bet(const Player& player, int32_t amount) -> void;
            auto check(const Player& player) -> void;
            auto fold(const Player& player) -> void;

            [[nodiscard]] auto toJson() const -> json;

        private:
            round_actions_t          _actions;
            Board                    _board;
            Hand                     _hand;
            int32_t                  _pot            = 0;
            int32_t                  _bet            = 0;
            Street                   _currentStreet  = Street::PREFLOP;
            time_point<system_clock> _lastActionTime = system_clock::now();
            const Player*            _winner         = nullptr;

            auto _getAndResetLastActionTime() -> seconds;
    };
}  // namespace GameHandler
