#pragma once

#include "Hand.hpp"

namespace GameHandler {
    constexpr int8_t BOARD_CARDS_NUMBER = 5;
    constexpr int8_t FLOP_CARDS_NUMBER  = 3;
    constexpr int8_t STRAIGHT_SIZE      = 5;
    constexpr int8_t FLUSH_SIZE         = 5;

    enum class HandRank : int8_t { HIGH_CARD = 0, PAIR, TWO_PAIR, TRIPS, STRAIGHT, FLUSH, FULL, QUADS, STRAIGHT_FLUSH };

    class Board {
        public:
            using board_t            = std::array<Card, BOARD_CARDS_NUMBER>;
            using rank_frequencies_t = std::array<int8_t, RANK_CARDS_NUMBER + 1>;
            using suit_frequencies_t = std::array<int8_t, SUIT_CARDS_NUMBER>;

            Board()                   = default;
            Board(const Board& other) = default;
            Board(Board&& other) noexcept { *this = std::move(other); };
            explicit Board(const board_t& cards) { setCards(cards); }

            virtual ~Board() = default;

            auto operator=(const Board& other) -> Board& = default;
            auto operator=(Board&& other) noexcept -> Board&;

            [[nodiscard]] auto getCards() const -> board_t { return _cards; }
            [[nodiscard]] auto hasPossibleStraight() const -> bool { return _possibleStraight; }
            [[nodiscard]] auto hasPossibleFlush() const -> bool { return _possibleFlush; }
            [[nodiscard]] auto hasPossibleFlushDraw() const -> bool { return _possibleFlushDraw; }
            [[nodiscard]] auto hasPair() const -> bool { return _pair; }
            [[nodiscard]] auto hasTwoPair() const -> bool { return _twoPair; }
            [[nodiscard]] auto hasTrips() const -> bool { return _trips; }
            [[nodiscard]] auto hasStraight() const -> bool { return _straight; }
            [[nodiscard]] auto hasFlush() const -> bool { return _flush; }
            [[nodiscard]] auto hasFull() const -> bool { return _full; }
            [[nodiscard]] auto hasQuads() const -> bool { return _quads; }
            [[nodiscard]] auto hasStraightFlush() const -> bool { return _straightFlush; }

            auto setCards(const board_t& cards) -> void;
            auto setFlop(const std::array<Card, FLOP_CARDS_NUMBER>& cards) -> void;
            auto setTurn(const Card& card) -> void;
            auto setRiver(const Card& card) -> void;

            auto getHighCardRank() -> Card::Rank;
            auto getHandRank(const Hand& hand) -> HandRank;

            [[nodiscard]] auto toJson() const -> json;
            [[nodiscard]] auto toDetailedJson() const -> json;

        private:
            board_t            _cards;
            rank_frequencies_t _rankFrequencies{};
            suit_frequencies_t _suitFrequencies{};
            bool               _possibleStraight  = false;
            bool               _possibleFlush     = false;
            bool               _possibleFlushDraw = false;
            bool               _pair              = false;
            bool               _twoPair           = false;
            bool               _trips             = false;
            bool               _straight          = false;
            bool               _flush             = false;
            bool               _full              = false;
            bool               _quads             = false;
            bool               _straightFlush     = false;

            auto _countPossibleStraights(int8_t otherCards, std::optional<rank_frequencies_t> frequencies = std::nullopt) -> int8_t;
            auto _computeRankFrequencies(std::optional<Hand> hand = std::nullopt) -> rank_frequencies_t;
            auto _computeSuitFrequencies(std::optional<Hand> hand = std::nullopt) -> suit_frequencies_t;

            auto _hasPossibleFlush() -> bool;
            auto _hasPossibleFlushDraw() -> bool;
            auto _hasPossibleStraight() -> bool;
            auto _hasPaire() -> bool;
            auto _hasDoublePaire() -> bool;
            auto _hasTrips() -> bool;
            auto _hasStraight() -> bool;
            auto _hasFlush() -> bool;
            auto _hasFull() -> bool;
            auto _hasQuads() -> bool;

            auto _updateStats() -> void;
    };
}  // namespace GameHandler