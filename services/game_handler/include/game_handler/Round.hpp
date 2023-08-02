#pragma once

#include <stack>

#include <game_handler/Board.hpp>
#include <game_handler/RoundAction.hpp>

namespace GameHandler {
    using std::chrono::system_clock;
    using std::chrono::time_point;

    using ActionType = RoundAction::ActionType;

    static const int32_t STREET_NUMBER = 5;

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
            uint32_t   playerNum      = 0;
            ActionType lastAction     = ActionType::NONE;
            int32_t    totalBet       = 0;
            int32_t    totalStreetBet = 0;
            int32_t    maxWinnable    = 0;
            int32_t    won            = 0;
            int32_t    initialStack   = 0;
            bool       inRound        = true;
            bool       isAllIn        = false;

            explicit PlayerStatus(const Player& player) :
                playerNum(player.getNumber()), initialStack(player.getStack()), inRound(!player.isEliminated()){};

            auto payBlind(int32_t amount) -> void {
                // @todo what if player cannot pay the blind?
                auto payAmount = std::min(amount, initialStack);

                isAllIn = payAmount == initialStack;
                totalBet += payAmount;
                totalStreetBet += payAmount;
            }

            auto hasBet(int32_t amount) -> void {
                isAllIn = (amount + totalBet) == initialStack;
                totalBet += amount;
                totalStreetBet += amount;
                lastAction = ActionType::BET;
            }

            auto hasCalled(int32_t amount) -> void {
                isAllIn = (amount + totalBet) == initialStack;
                totalBet += amount;
                totalStreetBet += amount;
                lastAction = ActionType::CALL;
            }

            auto hasChecked() -> void { lastAction = ActionType::CHECK; }

            auto hasFolded() -> void {
                inRound    = false;
                lastAction = ActionType::FOLD;
            }

            auto streetReset() -> void {
                totalStreetBet = 0;
                lastAction     = ActionType::NONE;
            }
    };

    class Round {
        public:
            using round_actions_t    = std::array<std::vector<RoundAction>, STREET_NUMBER>;
            using ranking_t          = std::stack<std::vector<Player>>;
            using players_status_t   = std::array<PlayerStatus, 3>;
            using players_status_ptr = std::unique_ptr<players_status_t>;

            enum Street : int32_t { PREFLOP = 0, FLOP, TURN, RIVER, SHOWDOWN };

            Round(const Blinds& blinds, std::array<Player, 3>& players);
            Round(const Round& other) = delete;
            Round(Round&& other) noexcept { *this = std::move(other); };

            virtual ~Round() = default;

            auto operator=(const Round& other) -> Round& = delete;
            auto operator=(Round&& other) noexcept -> Round&;

            [[nodiscard]] auto getBoard() -> Board& { return _board; }
            [[nodiscard]] auto getPot() const -> int32_t { return _pot; }
            [[nodiscard]] auto isInProgress() const -> bool { return !_ended; }
            [[nodiscard]] auto isInitialized() const -> bool { return _initialized; }

            auto call(uint32_t playerNum, int32_t amount) -> void;
            auto bet(uint32_t playerNum, int32_t amount) -> void;
            auto check(uint32_t playerNum) -> void;
            auto fold(uint32_t playerNum) -> void;
            auto showdown() -> void;

            [[nodiscard]] auto toJson() const -> json;

        private:
            round_actions_t          _actions;
            Board                    _board;
            ranking_t                _ranking;
            Hand                     _hand;
            Blinds                   _blinds         = Blinds(0, 0);
            int32_t                  _pot            = 0;
            int32_t                  _streePot       = 0;
            int32_t                  _frozenPot      = 0;
            Street                   _currentStreet  = Street::PREFLOP;
            time_point<system_clock> _lastActionTime = system_clock::now();
            std::array<Player, 3>*   _players        = nullptr;  // The Game class owns the players, so we use a pointer here
            players_status_ptr       _playersStatus  = nullptr;
            ActionType               _lastAction     = ActionType::NONE;
            bool                     _ended          = false;
            bool                     _initialized    = false;

            [[nodiscard]] auto _hasWon() const -> bool;
            [[nodiscard]] auto _toJson(const ranking_t& ranking) const -> json;
            [[nodiscard]] auto _getStacksVariation() const -> json;
            [[nodiscard]] auto _getPlayerStatus(uint32_t playerNum) const -> PlayerStatus;

            auto _getPlayer(uint32_t playerNum) -> Player&;
            auto _getNextPlayerNum(uint32_t playerNum) -> uint32_t;
            auto _getPlayerStatus(uint32_t playerNum) -> PlayerStatus&;
            auto _getAndResetLastActionTime() -> seconds;
            auto _determineStreetOver(const ActionType& currentPlayerAction) -> void;
            auto _determineRoundOver() -> void;
            auto _processRanking() -> void;
            auto _updateStacks() -> void;
            auto _payBlinds() -> void;
            auto _updatePlayersMaxWinnable() -> void;
            auto _endStreet() -> void;
    };
}  // namespace GameHandler
