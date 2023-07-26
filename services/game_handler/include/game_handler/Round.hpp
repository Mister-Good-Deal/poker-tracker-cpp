#pragma once

#include <game_handler/Board.hpp>
#include <game_handler/RoundAction.hpp>

namespace GameHandler {
    using std::chrono::system_clock;
    using std::chrono::time_point;

    using ActionType = RoundAction::ActionType;

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

    struct PlayerStatus {
        public:
            uint8_t    playerNum        = 0;
            ActionType lastAction       = ActionType::NONE;
            int32_t    lastActionAmount = 0;
            int32_t    totalBet         = 0;
            bool       inRound          = true;
            bool       isAllIn          = false;

            PlayerStatus() = default;
            explicit PlayerStatus(const Player& player) : playerNum(player.getNumber()){};

            auto hasBet(int32_t amount, int32_t stack) -> void {
                totalBet += amount;
                lastActionAmount = amount;
                isAllIn          = amount == stack;
                lastAction       = ActionType::BET;
            }

            auto hasCalled(int32_t amount, int32_t stack) -> void {
                totalBet += amount;
                lastActionAmount = amount;
                isAllIn          = amount == stack;
                lastAction       = ActionType::CALL;
            }

            auto hasChecked() -> void {
                lastActionAmount = 0;
                lastAction       = ActionType::CHECK;
            }

            auto hasFolded() -> void {
                inRound    = false;
                lastAction = ActionType::FOLD;
            }

            auto reset() -> void {
                inRound          = true;
                isAllIn          = false;  // @todo Check auto all in on SB / BB
                lastActionAmount = 0;
                lastAction       = ActionType::NONE;
            }
    };

    class Round {
        public:
            using round_actions_t = std::array<std::vector<RoundAction>, STREET_NUMBER>;

            enum Street : int8_t { PREFLOP = 0, FLOP, TURN, RIVER, SHOWDOWN };

            Round(Hand hand, const Blinds& blinds, std::array<Player, 3>& players);
            Round(const Round& other) = default;
            Round(Round&& other) noexcept { *this = std::move(other); };

            virtual ~Round() = default;

            auto operator=(const Round& other) -> Round& = default;
            auto operator=(Round&& other) noexcept -> Round&;

            [[nodiscard]] auto getBoard() -> Board& { return _board; }
            [[nodiscard]] auto getPot() const -> int32_t { return _pot; }
            [[nodiscard]] auto isInProgress() const -> bool { return !_ended; }
            [[nodiscard]] auto isInitialized() const -> bool { return _initialized; }

            auto init(const Hand& hand, const Blinds& blinds, int32_t pot, std::array<Player, 3>& players) -> void;

            auto call(uint8_t playerNum, int32_t amount) -> void;
            auto bet(uint8_t playerNum, int32_t amount) -> void;
            auto check(uint8_t playerNum) -> void;
            auto fold(uint8_t playerNum) -> void;
            auto showdown() -> void;

            [[nodiscard]] auto toJson() const -> json;

        private:
            round_actions_t             _actions;
            Board                       _board;
            Hand                        _hand;
            Blinds                      _blinds         = Blinds(0, 0);
            int32_t                     _pot            = 0;
            Street                      _currentStreet  = Street::PREFLOP;
            time_point<system_clock>    _lastActionTime = system_clock::now();
            std::array<Player, 3>*      _players        = nullptr;  // The Game class owns the players, so we use a pointer here
            std::array<PlayerStatus, 3> _playersStatus;
            ActionType                  _lastAction  = ActionType::NONE;
            Player*                     _winner      = nullptr;
            bool                        _ended       = false;
            bool                        _initialized = false;
            
            auto _getPlayer(uint8_t playerNum) -> Player&;
            auto _getPlayerStatus(uint8_t playerNum) -> PlayerStatus&;
            auto _getAndResetLastActionTime() -> seconds;
            auto _processStreet(const ActionType& currentPlayerAction) -> void;
            auto _determineRoundOver() -> void;
            auto _endStreet() -> void;
    };
}  // namespace GameHandler
