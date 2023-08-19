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
            auto getPlayers() -> std::array<Player, 3>& { return _players; };

            [[nodiscard]] auto hasNoRound() const -> bool { return _rounds.empty(); };
            [[nodiscard]] auto isOver() const -> bool { return _ended; };
            [[nodiscard]] auto getBuyIn() const -> int32_t { return _buyIn; };
            [[nodiscard]] auto getMultipliers() const -> int32_t { return _multipliers; };
            [[nodiscard]] auto getInitialStack() const -> int32_t { return _initialStack; };
            [[nodiscard]] auto getPlayer(int32_t playerNum) const -> const Player&;

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
            bool                     _complete     = true;
            bool                     _ended        = false;

            [[nodiscard]] auto _computeBalance() const -> int32_t;
            [[nodiscard]] auto _isGameWon() const -> bool;
    };
}  // namespace GameHandler

// Custom formatter for Position and Blinds
namespace fmt {
    using GameHandler::Game;

    template<> struct formatter<Game> : formatter<string_view> {
            template<typename FormatContext> auto format(const Game& game, FormatContext& ctx) const {
                return fmt::format_to(ctx.out(),
                                      "Game information:\n\tBuy-in: {}\n\tPrize pool: {}\n\tInitial stack: "
                                      "{}\n\tPlayer 1: {}\n\tPlayer 2: {}\n\tPlayer 3: {}",
                                      game.getBuyIn(),
                                      game.getBuyIn() * game.getMultipliers(),
                                      game.getInitialStack(),
                                      game.getPlayer(1).getName(),
                                      game.getPlayer(2).getName(),
                                      game.getPlayer(3).getName());
            }
    };
}  // namespace fmt

// Registered as safe to copy for Quill logger
namespace quill {
    template<> struct copy_loggable<GameHandler::Game> : std::true_type {};
}  // namespace quill
