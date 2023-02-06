#include "Board.hpp"

#include "Round.hpp"

namespace GameHandler {
    using std::ranges::copy;
    using std::ranges::count;
    using std::ranges::for_each;
    using std::ranges::max_element;
    using std::views::counted;

    using enum Card::Rank;

    auto Board::operator=(Board&& other) noexcept -> Board& {
        if (this != &other) {
            cards            = std::move(other.cards);
            rankFrequencies  = std::move(other.rankFrequencies);
            suitFrequencies  = std::move(other.suitFrequencies);
            gutShots         = other.gutShots;
            flushDraw        = other.flushDraw;
            possibleStraight = other.possibleStraight;
            possibleFlush    = other.possibleFlush;
            paire            = other.paire;
            doublePaire      = other.doublePaire;
            trips            = other.trips;
            straight         = other.straight;
            flush            = other.flush;
            full             = other.full;
            quads            = other.quads;
            straightFlush    = other.straightFlush;
        }

        return *this;
    }

    auto Board::setCards(const Board::board_t& allCards) -> void {
        cards = allCards;

        updateStats();
    }

    auto Board::setFlop(const std::array<Card, FLOP_CARDS_NUMBER>& flopCards) -> void {
        copy(flopCards.begin(), flopCards.end(), cards.begin());

        updateStats();
    }

    auto Board::setTurn(const Card& card) -> void {
        constexpr int8_t TURN_CARD_INDEX = 3;

        cards[TURN_CARD_INDEX] = card;

        updateStats();
    }
    auto Board::setRiver(const Card& card) -> void {
        constexpr int8_t RIVER_CARD_INDEX = 4;

        cards[RIVER_CARD_INDEX] = card;

        updateStats();
    }

    auto Board::toJson() const -> json {
        auto cardsArray = json::array();

        for_each(cards, [&cardsArray](const Card& card) { cardsArray.emplace_back(card.toJson()); });

        return {{"cards", cardsArray},
                {"properties",
                 {{"gutShots", gutShots},
                  {"flushDraw", flushDraw},
                  {"possibleStraight", possibleStraight},
                  {"possibleFlush", possibleFlush},
                  {"paire", paire},
                  {"doublePaire", doublePaire},
                  {"trips", trips},
                  {"straight", straight},
                  {"flush", flush},
                  {"full", full},
                  {"quads", quads},
                  {"straightFlush", straightFlush}}}};
    }

    auto Board::computeRankFrequencies() -> rank_frequencies_t {
        rank_frequencies_t frequences{};
        // Construct cards rank frequences sequence
        for_each(cards, [&frequences](const Card& card) {
            if (card.getRank() != UNDEFINED) {
                frequences.at(static_cast<int8_t>(card.getRank()))++;
            }
        });

        return frequences;
    }

    auto Board::computeSuitFrequencies() -> suit_frequencies_t {
        suit_frequencies_t frequences{};
        // Construct cards suit frequences sequence
        for_each(cards, [&frequences](const Card& card) {
            if (card.getSuit() != Card::Suit::UNKNOWN) {
                frequences.at(static_cast<int8_t>(card.getSuit()))++;
            }
        });

        return frequences;
    }

    // @todo check std::views::adjacent_transform compilers implementation status
    auto Board::countPossibleStraights(int8_t additionalCards) -> int8_t {
        int8_t possibleStraights = 0;

        // Special Ace case
        rankFrequencies[0] = rankFrequencies[static_cast<int8_t>(ACE)];

        const int8_t WINDOW_SIZE = 5;

        for (int8_t i = 0; i < rankFrequencies.size() - WINDOW_SIZE + 1; ++i) {
            auto window = counted(rankFrequencies.begin() + i, WINDOW_SIZE);

            if (count(window, 1) >= STRAIGHT_SIZE - additionalCards) {
                possibleStraights++;
            }
        }

        // Reset special Ace case
        rankFrequencies[0] = 0;

        return possibleStraights;
    }

    auto Board::getHighCardRank() -> Card::Rank {
        return max_element(cards, [](const Card& A, const Card& B) { return A.getRank() < B.getRank(); })->getRank();
    }

    auto Board::isPaire() -> bool { return count(rankFrequencies, 2) >= 1; }
    auto Board::isDoublePaire() -> bool { return count(rankFrequencies, 2) >= 2; }
    auto Board::isTrips() -> bool { return count(rankFrequencies, 3) == 1; }
    auto Board::isStraight() -> bool { return countPossibleStraights(0) == 1; }
    auto Board::countGutShots() -> int8_t { return straight ? 0 : countPossibleStraights(1); }
    auto Board::isStraightPossible() -> bool { return straight || countPossibleStraights(2) > 0; }
    auto Board::isFlush() -> bool { return count(suitFrequencies, FLUSH_SIZE) == 1; }
    auto Board::isFlushDraw() -> bool { return !flush && count(suitFrequencies, 2) >= 1; }
    auto Board::isFlushPossible() -> bool { return flush || count(suitFrequencies, 3) == 1; }
    auto Board::isFull() -> bool { return count(rankFrequencies, 2) >= 1 && count(rankFrequencies, 3) >= 1; }
    auto Board::isQuads() -> bool { return count(rankFrequencies, 4) == 1; }

    auto Board::updateStats() -> void {
        rankFrequencies = computeRankFrequencies();
        suitFrequencies = computeSuitFrequencies();
        // The order is important, possible flush or possible straight use flush and straight values for optimisation
        paire            = isPaire();
        doublePaire      = isDoublePaire();
        trips            = isTrips();
        straight         = isStraight();
        possibleStraight = isStraightPossible();
        gutShots         = countGutShots();
        flush            = isFlush();
        possibleFlush    = isFlushPossible();
        flushDraw        = isFlushDraw();
        full             = isFull();
        quads            = isQuads();
        straightFlush    = straight && flush;
    }
}  // namespace GameHandler
