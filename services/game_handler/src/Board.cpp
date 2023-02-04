#include "Board.hpp"

#include "Round.hpp"

namespace GameHandler {
    using std::ranges::copy;
    using std::ranges::count;
    using std::ranges::fill;
    using std::ranges::for_each;
    using std::ranges::max_element;
    using std::ranges::transform;
    using std::views::counted;

    using enum Card::Rank;
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

    auto Board::getHighCardRank() -> Card::Rank {
        return max_element(cards, [](const Card& A, const Card& B) { return A.getRank() < B.getRank(); })->getRank();
    }

    auto Board::isFlushDraw() -> bool { return false; }

    // @todo check std::views::adjacent_transform compilers implementation status
    auto Board::countPossibleStraights(int8_t additionalCards) -> int8_t {
        int8_t                                    possibleStraights = 0;
        std::array<int8_t, RANK_CARDS_NUMBER + 1> frequences{};
        // Construct cards frequences sequence
        for_each(cards, [&frequences](const Card& card) { frequences.at(static_cast<int8_t>(card.getRank()))++; });
        // Special Ace case
        frequences[0] = frequences[static_cast<int8_t>(ACE)];

        const int8_t WINDOW_SIZE = 5;

        for (int8_t i = 0; i < frequences.size() - WINDOW_SIZE; ++i) {
            auto window = counted(frequences.begin() + i, WINDOW_SIZE);

            if (count(window, 1) >= STRAIGHT_SIZE - additionalCards) {
                possibleStraights++;
            }
        }

        return possibleStraights;
    }

    auto Board::isStraightPossible() -> bool { return countPossibleStraights(2) > 0; }
}  // namespace GameHandler
