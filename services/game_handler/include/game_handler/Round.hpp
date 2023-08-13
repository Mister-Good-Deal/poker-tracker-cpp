#pragma once

#include <stack>
#include <utility>

#include <game_handler/Board.hpp>
#include <game_handler/RoundAction.hpp>

namespace GameHandler {
    using std::chrono::system_clock;
    using std::chrono::time_point;

    using ActionType = RoundAction::ActionType;

    static const int32_t STREET_NUMBER = 5;

    enum Position : int32_t { DEALER = 0, SMALL_BLIND, BIG_BLIND };

    struct Blinds {
        public:
            int32_t smallBlind = 0;
            int32_t bigBlind   = 0;

            Blinds() = default;
            Blinds(int32_t smallBlind, int32_t bigBlind) : smallBlind(smallBlind), bigBlind(bigBlind) {}

            [[nodiscard]] auto SB() const -> int32_t { return smallBlind; }
            [[nodiscard]] auto BB() const -> int32_t { return bigBlind; }
    };

    struct PlayerStatus : public Player {
        public:
            ActionType lastAction     = ActionType::NONE;
            int32_t    totalBet       = 0;
            int32_t    totalStreetBet = 0;
            int32_t    maxWinnable    = 0;
            int32_t    endRoundStack  = 0;
            bool       inRound        = true;
            bool       isAllIn        = false;
            Hand       hand           = Hand();
            Position   position;

            explicit PlayerStatus(const Player& player, int32_t dealerNumber) :
                position(static_cast<Position>((player.getNumber() - dealerNumber + 3) % 3)), inRound(!player.isEliminated()),
                endRoundStack(player.getStack()), Player(player){};

            auto payBlind(int32_t amount) -> void {
                // @todo what if player cannot pay the blind?
                auto payAmount = std::min(amount, getStack());

                isAllIn = payAmount == getStack();
                totalBet += payAmount;
                totalStreetBet += payAmount;
            }

            auto hasBet(int32_t amount) -> void {
                isAllIn = (amount + totalBet) == getStack();
                totalBet += amount;
                totalStreetBet += amount;
                lastAction = ActionType::BET;
            }

            auto hasCalled(int32_t amount) -> void {
                isAllIn = (amount + totalBet) == getStack();
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
            using ranking_t          = std::stack<std::vector<int32_t>>;
            using players_status_t   = std::array<PlayerStatus, 3>;
            using players_status_ptr = std::unique_ptr<players_status_t>;

            enum Street : int32_t { PREFLOP = 0, FLOP, TURN, RIVER, SHOWDOWN };

            Round(const Blinds& blinds, std::array<Player, 3>& players, Hand hand, int32_t dealerNumber);
            Round(const Round& other) = delete;
            Round(Round&& other) noexcept { *this = std::move(other); };

            virtual ~Round() = default;

            auto operator=(const Round& other) -> Round& = delete;
            auto operator=(Round&& other) noexcept -> Round&;

            [[nodiscard]] auto getBoard() -> Board& { return _board; }
            [[nodiscard]] auto getPot() const -> int32_t { return _pot; }
            [[nodiscard]] auto isInProgress() const -> bool { return !_ended; }

            auto call(uint32_t playerNum, int32_t amount) -> void;
            auto bet(uint32_t playerNum, int32_t amount) -> void;
            auto check(uint32_t playerNum) -> void;
            auto fold(uint32_t playerNum) -> void;
            auto showdown() -> void;
            auto setPlayer2Hand(const Hand& hand) -> void { _getPlayerStatus(2).hand = hand; }
            auto setPlayer3Hand(const Hand& hand) -> void { _getPlayerStatus(3).hand = hand; }

            [[nodiscard]] auto toJson() const -> json;

        private:
            round_actions_t          _actions;
            Board                    _board;
            ranking_t                _ranking;
            Blinds                   _blinds         = Blinds(0, 0);
            int32_t                  _pot            = 0;
            int32_t                  _streetPot      = 0;
            int32_t                  _frozenPot      = 0;
            Street                   _currentStreet  = Street::PREFLOP;
            time_point<system_clock> _lastActionTime = system_clock::now();
            std::array<Player, 3>*   _players        = nullptr;  // The Game class owns the players, so we use a pointer here
            players_status_ptr       _playersStatus  = nullptr;
            ActionType               _lastAction     = ActionType::NONE;
            bool                     _ended          = false;

            [[nodiscard]] auto _hasWon() const -> bool;
            [[nodiscard]] auto _toJson(const ranking_t& ranking) const -> json;
            [[nodiscard]] auto _getStacksVariation() const -> json;
            [[nodiscard]] auto _getPlayer(uint32_t playerNum) const -> Player;
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
            auto _endRound() -> void;
    };
}  // namespace GameHandler

// Custom formatter for Position
namespace fmt {
    using GameHandler::Position;

    template<>
    struct formatter<Position> : formatter<string_view> {
            // parse is inherited from formatter<string_view>.
            template<typename FormatContext>
            auto format(Position position, FormatContext& ctx) const {
                string_view name = "unknown";

                switch (position) {
                    case Position::DEALER: name = "dealer"; break;
                    case Position::SMALL_BLIND: name = "small_blind"; break;
                    case Position::BIG_BLIND: name = "big_blind"; break;
                }

                return formatter<string_view>::format(name, ctx);
            }
    };
}  // namespace fmt
