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
            int32_t    initialStack   = 0;
            bool       inRound        = true;
            bool       isAllIn        = false;
            Hand       hand           = Hand();
            Position   position;

            explicit PlayerStatus(const Player& player, int32_t dealerNumber) :
              position(static_cast<Position>((player.getNumber() - dealerNumber + 3) % 3)), inRound(!player.isEliminated()),
              initialStack(player.getStack()), Player(player){};

            auto payBlind(int32_t amount) -> void {
                auto payAmount = std::min(amount, getStack());

                totalBet       += payAmount;
                totalStreetBet += payAmount;
                isAllIn         = totalBet == initialStack;

                setStack(initialStack - totalBet);
            }

            auto hasBet(int32_t amount) -> void {
                totalBet       += amount;
                totalStreetBet += amount;
                isAllIn         = totalBet == initialStack;
                lastAction      = ActionType::BET;

                setStack(initialStack - totalBet);
            }

            auto hasRaised(int32_t amount) -> void {
                totalBet       += amount - totalStreetBet;
                totalStreetBet  = amount;
                isAllIn         = totalBet == initialStack;
                lastAction      = ActionType::RAISE;

                setStack(initialStack - totalBet);
            }

            auto hasCalled(int32_t amount) -> void {
                totalBet       += amount;
                totalStreetBet += amount;
                isAllIn         = totalBet == initialStack;
                lastAction      = ActionType::CALL;

                setStack(initialStack - totalBet);
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

            auto winChips(int32_t chips) -> void { setStack(getStack() + chips); }
    };

    class Round {
        public:
            using round_actions_t    = std::array<std::vector<RoundAction>, STREET_NUMBER>;
            using ranking_t          = std::stack<std::vector<int32_t>>;
            using players_status_t   = std::array<PlayerStatus, 3>;
            using players_status_ptr = std::unique_ptr<players_status_t>;

            enum Street : int32_t { PREFLOP = 0, FLOP, TURN, RIVER, SHOWDOWN };

            Round(const Blinds& blinds, std::array<Player, 3>& players, Hand hand, int32_t dealerNumber);
            Round(const Round& other) { *this = other; }
            Round(Round&& other) noexcept { *this = std::move(other); };

            virtual ~Round() = default;

            auto operator=(const Round& other) -> Round&;
            auto operator=(Round&& other) noexcept -> Round&;

            [[nodiscard]] auto getBoard() -> Board& { return _board; }
            [[nodiscard]] auto getPot() const -> int32_t { return _pot; }
            [[nodiscard]] auto getLastAction() -> RoundAction { return _currentAction; };
            [[nodiscard]] auto getCurrentPlayerStack(int32_t playerNum) const -> int32_t;
            [[nodiscard]] auto getNextPlayerNum(int32_t playerNum) const -> int32_t;
            [[nodiscard]] auto isInProgress() const -> bool { return !_ended; }
            [[nodiscard]] auto waitingShowdown() const -> bool;
            [[nodiscard]] auto getInRoundPlayersNum() const -> std::vector<int32_t>;
            [[nodiscard]] auto getPlayerHand(int32_t playerNum) const -> Hand { return _getPlayerStatus(playerNum).hand; }
            [[nodiscard]] auto getCurrentStreet() const -> Street { return _currentStreet; }

            auto call(int32_t playerNum) -> void;
            auto bet(int32_t playerNum, int32_t amount) -> void;
            auto raise(int32_t playerNum, int32_t amount) -> void;
            auto check(int32_t playerNum) -> void;
            auto fold(int32_t playerNum) -> void;
            auto showdown() -> void;
            auto setPlayerHand(const Hand& hand, int32_t playerNum) -> void { _getPlayerStatus(playerNum).hand = hand; }

            [[nodiscard]] auto toJson() const -> json;

        private:
            round_actions_t          _actions;
            Board                    _board;
            ranking_t                _ranking;
            Blinds                   _blinds         = Blinds(0, 0);
            int32_t                  _pot            = 0;
            int32_t                  _streetPot      = 0;
            int32_t                  _frozenPot      = 0;
            int32_t                  _dealerNumber   = 0;
            int32_t                  _lastBetOrRaise = 0;
            Hand                     _hand           = Hand();
            Street                   _currentStreet  = Street::PREFLOP;
            time_point<system_clock> _lastActionTime = system_clock::now();
            std::array<Player, 3>*   _players        = nullptr;  // The Game class owns the players, so we use a pointer here
            players_status_ptr       _playersStatus  = nullptr;
            RoundAction              _currentAction  = RoundAction();
            RoundAction              _lastAction     = RoundAction();
            bool                     _ended          = false;

            [[nodiscard]] auto _hasWon() const -> bool;
            [[nodiscard]] auto _toJson(const ranking_t& ranking) const -> json;
            [[nodiscard]] auto _getStacksVariation() const -> json;
            [[nodiscard]] auto _getPlayerStatus(int32_t playerNum) const -> PlayerStatus;
            [[nodiscard]] auto _isStreetOver() const -> bool;

            auto _getPlayer(int32_t playerNum) -> Player&;
            auto _getPlayerStatus(int32_t playerNum) -> PlayerStatus&;
            auto _getAndResetLastActionTime() -> seconds;
            auto _determineStreetOver() -> void;
            auto _determineRoundOver() -> void;
            auto _processRanking() -> void;
            auto _updateStacks() -> void;
            auto _payBlinds() -> void;
            auto _updatePlayersMaxWinnable() -> void;
            auto _endStreet() -> void;
            auto _endRound() -> void;
    };
}  // namespace GameHandler

// Custom formatter for Position and Blinds
namespace fmt {
    using GameHandler::Blinds;
    using GameHandler::Position;

    template<> struct formatter<Position> : formatter<string_view> {
            template<typename FormatContext> auto format(Position position, FormatContext& ctx) const {
                string_view name = "unknown";

                switch (position) {
                    case Position::DEALER: name = "dealer"; break;
                    case Position::SMALL_BLIND: name = "small_blind"; break;
                    case Position::BIG_BLIND: name = "big_blind"; break;
                }

                return formatter<string_view>::format(name, ctx);
            }
    };

    template<> struct formatter<Blinds> : formatter<string_view> {
            template<typename FormatContext> auto format(const Blinds& blinds, FormatContext& ctx) const {
                return fmt::format_to(ctx.out(), "({}-{})", blinds.SB(), blinds.BB());
            }
    };
}  // namespace fmt

// Registered as safe to copy for Quill logger
namespace quill {
    template<> struct copy_loggable<GameHandler::Blinds> : std::true_type {};
}  // namespace quill
