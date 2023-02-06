#pragma once

#include "Card.hpp"

namespace GameHandler {
    constexpr int8_t BOARD_CARDS_NUMBER = 5;
    constexpr int8_t FLOP_CARDS_NUMBER  = 3;
    constexpr int8_t STRAIGHT_SIZE      = 5;
    constexpr int8_t FLUSH_SIZE         = 5;

    class Board {
        public:
            using board_t            = std::array<Card, BOARD_CARDS_NUMBER>;
            using rank_frequencies_t = std::array<int8_t, RANK_CARDS_NUMBER + 1>;
            using suit_frequencies_t = std::array<int8_t, SUIT_CARDS_NUMBER>;

            Board()                   = default;
            Board(const Board& other) = default;
            Board(Board&& other) noexcept { *this = std::move(other); };
            explicit Board(const board_t& allCards) { setCards(allCards); }

            virtual ~Board() = default;

            auto operator=(const Board& other) -> Board& = default;
            auto operator=(Board&& other) noexcept -> Board&;

            [[nodiscard]] auto getGutShots() const -> int8_t { return gutShots; }
            [[nodiscard]] auto hasFlushDraw() const -> bool { return flushDraw; }
            [[nodiscard]] auto hasPossibleStraight() const -> bool { return possibleStraight; }
            [[nodiscard]] auto hasPossibleFlush() const -> bool { return possibleFlush; }
            [[nodiscard]] auto hasPaire() const -> bool { return paire; }
            [[nodiscard]] auto hasDoublePaire() const -> bool { return doublePaire; }
            [[nodiscard]] auto hasTrips() const -> bool { return trips; }
            [[nodiscard]] auto hasStraight() const -> bool { return straight; }
            [[nodiscard]] auto hasFlush() const -> bool { return flush; }
            [[nodiscard]] auto hasFull() const -> bool { return full; }
            [[nodiscard]] auto hasQuads() const -> bool { return quads; }
            [[nodiscard]] auto hasStraightFlush() const -> bool { return straightFlush; }

            auto setCards(const board_t& allCards) -> void;
            auto setFlop(const std::array<Card, FLOP_CARDS_NUMBER>& flopCards) -> void;
            auto setTurn(const Card& card) -> void;
            auto setRiver(const Card& card) -> void;

            auto getHighCardRank() -> Card::Rank;

            [[nodiscard]] auto toJson() const -> json;

        protected:
            auto countPossibleStraights(int8_t additionalCards) -> int8_t;
            auto computeRankFrequencies() -> rank_frequencies_t;
            auto computeSuitFrequencies() -> suit_frequencies_t;

        private:
            board_t            cards;
            rank_frequencies_t rankFrequencies{};
            suit_frequencies_t suitFrequencies{};
            int8_t             gutShots         = 0;
            bool               flushDraw        = false;
            bool               possibleStraight = false;
            bool               possibleFlush    = false;
            bool               paire            = false;
            bool               doublePaire      = false;
            bool               trips            = false;
            bool               straight         = false;
            bool               flush            = false;
            bool               full             = false;
            bool               quads            = false;
            bool               straightFlush    = false;

            auto countGutShots() -> int8_t;
            auto isFlushDraw() -> bool;
            auto isFlushPossible() -> bool;
            auto isStraightPossible() -> bool;
            auto isPaire() -> bool;
            auto isDoublePaire() -> bool;
            auto isTrips() -> bool;
            auto isStraight() -> bool;
            auto isFlush() -> bool;
            auto isFull() -> bool;
            auto isQuads() -> bool;

            auto updateStats() -> void;
    };
}  // namespace GameHandler