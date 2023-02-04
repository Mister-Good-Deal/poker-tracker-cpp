#pragma once

#include "Card.hpp"

namespace GameHandler {
    constexpr int8_t BOARD_CARDS_NUMBER = 5;
    constexpr int8_t FLOP_CARDS_NUMBER  = 3;
    constexpr int8_t STRAIGHT_SIZE      = 5;

    class Board {
        public:
            using board_t = std::array<Card, BOARD_CARDS_NUMBER>;

            Board()                   = default;
            Board(const Board& other) = default;
            Board(Board&& other) noexcept { *this = std::move(other); };

            virtual ~Board() = default;

            auto operator=(const Board& other) -> Board& = default;
            auto operator=(Board&& other) noexcept -> Board&;

            auto setCards(const board_t& allCards) -> void { cards = allCards; };
            auto setFlop(const std::array<Card, FLOP_CARDS_NUMBER>& flopCards) -> void;
            auto setTurn(const Card& card) -> void;
            auto setRiver(const Card& card) -> void;

            auto getHighCardRank() -> Card::Rank;

        protected:
            auto countPossibleStraights(int8_t additionalCards) -> int8_t;

        private:
            board_t cards;
            int8_t  gutShots         = 0;
            bool    flushDraw        = false;
            bool    possibleStraight = false;
            bool    paired           = false;
            bool    doublePaired     = false;
            bool    trips            = false;
            bool    straight         = false;
            bool    flush            = false;
            bool    quads            = false;
            bool    straightFlush    = false;

            auto isFlushDraw() -> bool;
            auto isStraightPossible() -> bool;
    };
}  // namespace GameHandler