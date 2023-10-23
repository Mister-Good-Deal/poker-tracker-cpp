#pragma once

#include <stack>
#include <utility>

#include <game_handler/Board.hpp>
#include <game_handler/RoundAction.hpp>

namespace GameHandler {
    using std::chrono::system_clock;
    using std::chrono::time_point;

    using ActionType = RoundAction::ActionType;

    using enum ActionType;

    static const int32_t STREET_NUMBER = 5;

    enum Position : int32_t { DEALER = 0, SMALL_BLIND, BIG_BLIND };

    struct Blinds {
        public:
            int32_t smallBlind = 0;
            int32_t bigBlind   = 0;

            Blinds() = default;
            Blinds(int32_t smallBlind, int32_t bigBlind)
              : smallBlind(smallBlind)
              , bigBlind(bigBlind) {}

            [[nodiscard]] auto SB() const -> int32_t { return smallBlind; }
            [[nodiscard]] auto BB() const -> int32_t { return bigBlind; }
    };

    // Used to store the players stats statically in the json after the round is over
    struct PlayerRoundRecap {
            PlayerRoundRecap() = default;
            PlayerRoundRecap(int32_t playerNumber, int32_t startStack, int32_t endStack)
              : playerNumber(playerNumber)
              , startStack(startStack)
              , endStack(endStack) {}

            int32_t playerNumber;
            int32_t startStack;
            int32_t endStack;
    };

    struct PlayerStatus : public Player {
        public:
            ActionType lastAction     = NONE;
            int32_t    totalBet       = 0;
            int32_t    totalStreetBet = 0;
            int32_t    maxWinnable    = 0;
            int32_t    initialStack   = 0;
            bool       inRound        = true;
            bool       isAllIn        = false;
            Hand       hand           = Hand();
            Position   position;
            Player*    player;

            explicit PlayerStatus(Player* player, int32_t dealerNumber)
              : position(static_cast<Position>((player->getNumber() - dealerNumber + 3) % 3))
              , inRound(!player->isEliminated())
              , initialStack(player->getStack())
              , player(player)
              , Player(*player) {};

            auto winChips(int32_t chips) -> void { setStack(getStack() + chips); }
            auto payBigBlind(int32_t amount) -> void { _updateStatus(std::min(amount, getStack()), PAY_BIG_BLIND); }
            auto paySmallBlind(int32_t amount) -> void { _updateStatus(std::min(amount, getStack()), PAY_SMALL_BLIND); }
            auto hasBet(int32_t amount) -> void { _updateStatus(amount, BET); }
            auto hasRaised(int32_t amount) -> void { _updateStatus(amount, RAISE); }
            auto hasCalled(int32_t amount) -> void { _updateStatus(amount, CALL); }
            auto hasChecked() -> void { lastAction = CHECK; }

            auto hasFolded() -> void {
                inRound    = false;
                lastAction = FOLD;
            }

            auto streetReset() -> void {
                totalStreetBet = 0;
                lastAction     = NONE;
            }
            // Adapter pattern to Player
            [[nodiscard]] auto getName() const -> std::string override { return player->getName(); }
            [[nodiscard]] auto getNumber() const -> int32_t override { return player->getNumber(); }
            [[nodiscard]] auto getStack() const -> int32_t override { return player->getStack(); }
            [[nodiscard]] auto isEliminated() const -> bool override { return player->isEliminated(); }
            [[nodiscard]] auto isHero() const -> bool override { return player->isHero(); }

            auto setStack(int32_t stack) -> void override { player->setStack(stack); }
            auto bust() -> void override { player->bust(); }

        private:
            auto _updateStatus(int32_t amount, ActionType action) -> void {
                totalBet       += amount;
                totalStreetBet += amount;
                isAllIn         = totalBet == initialStack;

                if (action != PAY_BIG_BLIND && action != PAY_SMALL_BLIND) { lastAction = action; }

                setStack(initialStack - totalBet);
            }
    };

    class Round {
        public:
            using round_actions_t       = std::array<std::vector<RoundAction>, STREET_NUMBER>;
            using players_round_recap_t = std::array<PlayerRoundRecap, 3>;
            using ranking_t             = std::stack<std::vector<int32_t>>;
            using players_status_t      = std::array<PlayerStatus, 3>;
            using players_status_ptr    = std::unique_ptr<players_status_t>;

            enum Street : int32_t { PREFLOP = 0, FLOP, TURN, RIVER, SHOWDOWN };

            Round(const Blinds& blinds, std::array<Player, 3>& players, Hand hand, int32_t dealerNumber);
            Round(const Round& other) { *this = other; }
            Round(Round&& other) noexcept { *this = std::move(other); };

            virtual ~Round() = default;

            auto operator=(const Round& other) -> Round&;
            auto operator=(Round&& other) noexcept -> Round&;

            [[nodiscard]] auto getBoard() -> Board& { return _board; }
            [[nodiscard]] auto getBoard() const -> const Board& { return _board; }
            [[nodiscard]] auto getPot() const -> int32_t { return _pot; }
            [[nodiscard]] auto getLastAction() const -> RoundAction { return _currentAction; };
            [[nodiscard]] auto getCurrentPlayerStack(int32_t playerNum) const -> int32_t;
            [[nodiscard]] auto isInProgress() const -> bool { return !_ended; }
            [[nodiscard]] auto isNextActionTheLastStreetOne(int32_t playerNum) const -> bool;
            [[nodiscard]] auto waitingShowdown() const -> bool;
            [[nodiscard]] auto getInRoundPlayersNum() const -> std::vector<int32_t>;
            [[nodiscard]] auto getPlayerHand(int32_t playerNum) const -> Hand { return _getPlayerStatus(playerNum).hand; }
            [[nodiscard]] auto getCurrentStreet() const -> Street { return _currentStreet; }
            [[nodiscard]] auto getCurrentPlayerNum() const -> int32_t { return _currentPlayerNum; }

            auto call(int32_t playerNum) -> void;
            auto bet(int32_t playerNum, int32_t amount) -> void;
            auto raiseTo(int32_t playerNum, int32_t amount) -> void;
            auto check(int32_t playerNum) -> void;
            auto fold(int32_t playerNum) -> void;
            auto allIn(int32_t playerNum) -> void;
            auto showdown() -> void;
            auto setPlayerHand(const Hand& hand, int32_t playerNum) -> void { _getPlayerStatus(playerNum).hand = hand; }

            [[nodiscard]] auto toJson() const -> json;

            [[nodiscard]] static auto toJson(const ranking_t& ranking) -> json;
            [[nodiscard]] static auto getStacksVariation(const players_round_recap_t& playersRoundRecap) -> json;

        private:
            round_actions_t          _actions;
            Board                    _board;
            ranking_t                _ranking;
            players_round_recap_t    _playersRoundRecap = {};  // Used to store the players status statically in the json
            Blinds                   _blinds            = Blinds(0, 0);
            int32_t                  _pot               = 0;
            int32_t                  _streetPot         = 0;
            int32_t                  _frozenPot         = 0;
            int32_t                  _dealerNumber      = 0;
            int32_t                  _lastBetOrRaise    = 0;
            int32_t                  _currentPlayerNum  = 0;
            Hand                     _hand              = Hand();
            Street                   _currentStreet     = Street::PREFLOP;
            time_point<system_clock> _lastActionTime    = system_clock::now();
            std::array<Player, 3>*   _players           = nullptr;  // The Game class owns the players, so we use a pointer here
            players_status_ptr       _playersStatus     = nullptr;
            RoundAction              _currentAction     = RoundAction();
            RoundAction              _lastAction        = RoundAction();
            bool                     _ended             = false;

            [[nodiscard]] auto _hasWon() const -> bool;
            [[nodiscard]] auto _getPlayerStatus(int32_t playerNum) const -> PlayerStatus;
            [[nodiscard]] auto _getNextPlayerNum(int32_t playerNum) const -> int32_t;
            [[nodiscard]] auto _isStreetOver() const -> bool;

            auto _getPlayerStatus(int32_t playerNum) -> PlayerStatus&;
            auto _getAndResetLastActionTime() -> seconds;
            auto _setAction(int32_t playerNum, ActionType actionType, int32_t amount = 0) -> void;
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
    using GameHandler::Round;

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

    template<> struct formatter<Round::Street> : formatter<string_view> {
            template<typename FormatContext> auto format(const Round::Street& street, FormatContext& ctx) const {
                string_view name = "unknown";

                switch (street) {
                    case Round::PREFLOP: name = "pre-flop"; break;
                    case Round::FLOP: name = "flop"; break;
                    case Round::TURN: name = "turn"; break;
                    case Round::RIVER: name = "river"; break;
                    case Round::SHOWDOWN: name = "showdown"; break;
                }

                return formatter<string_view>::format(name, ctx);
            }
    };
}  // namespace fmt

// Registered as safe to copy for Quill logger
namespace quill {
    template<> struct copy_loggable<GameHandler::Blinds> : std::true_type {};
}  // namespace quill
