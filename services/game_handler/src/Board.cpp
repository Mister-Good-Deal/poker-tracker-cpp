#include "Board.hpp"

#include "Round.hpp"

namespace GameHandler {
    using std::ranges::copy;

    using enum Round::Street;

    namespace {
        constexpr int8_t TURN_CARD_INDEX  = 3;
        constexpr int8_t RIVER_CARD_INDEX = 4;
    };  // namespace

    auto Board::operator=(Board&& other) noexcept -> Board& {
        if (this != &other) {
            cards = std::move(other.cards);
        }

        return *this;
    }

    auto Board::setFlop(const std::array<Card, FLOP_CARDS_NUMBER>& flopCards) -> void {
        copy(flopCards.begin(), flopCards.end(), cards.begin());
    }

    auto Board::setTurn(const Card& card) -> void { cards[TURN_CARD_INDEX] = card; }
    auto Board::setRiver(const Card& card) -> void { cards[RIVER_CARD_INDEX] = card; }
}  // namespace GameHandler
