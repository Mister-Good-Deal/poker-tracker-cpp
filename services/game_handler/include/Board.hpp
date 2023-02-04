#pragma once

#include "Card.hpp"

namespace GameHandler {
    constexpr int8_t BOARD_CARDS_NUMBER = 5;
    constexpr int8_t FLOP_CARDS_NUMBER  = 5;

    class Board {
        public:
            Board()                   = default;
            Board(const Board& other) = default;
            Board(Board&& other) noexcept { *this = std::move(other); };

            virtual ~Board() = default;

            auto operator=(const Board& other) -> Board& = default;
            auto operator=(Board&& other) noexcept -> Board&;

            auto setFlop(const std::array<Card, FLOP_CARDS_NUMBER>& flopCards) -> void;
            auto setTurn(const Card& card) -> void;
            auto setRiver(const Card& card) -> void;

        private:
            std::array<Card, BOARD_CARDS_NUMBER> cards;
    };
}  // namespace GameHandler