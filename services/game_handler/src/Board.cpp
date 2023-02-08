#include "Board.hpp"

namespace GameHandler {
    using std::ranges::copy;
    using std::ranges::count;
    using std::ranges::count_if;
    using std::ranges::for_each;
    using std::ranges::max_element;
    using std::views::counted;

    using enum Card::Rank;
    using enum HandRank;

    auto Board::operator=(Board&& other) noexcept -> Board& {
        if (this != &other)
        {
            _cards             = std::move(other._cards);
            _rankFrequencies   = other._rankFrequencies;
            _suitFrequencies   = other._suitFrequencies;
            _possibleStraight  = other._possibleStraight;
            _possibleFlushDraw = other._possibleFlushDraw;
            _possibleFlush     = other._possibleFlush;
            _pair              = other._pair;
            _twoPair           = other._twoPair;
            _trips             = other._trips;
            _straight          = other._straight;
            _flush             = other._flush;
            _full              = other._full;
            _quads             = other._quads;
            _straightFlush     = other._straightFlush;
        }

        return *this;
    }

    auto Board::setCards(const Board::board_t& cards) -> void {
        _cards = cards;

        _updateStats();
    }

    auto Board::setFlop(const std::array<Card, FLOP_CARDS_NUMBER>& cards) -> void {
        copy(cards.begin(), cards.end(), _cards.begin());

        _updateStats();
    }

    auto Board::setTurn(const Card& card) -> void {
        constexpr int8_t TURN_CARD_INDEX = 3;

        _cards[TURN_CARD_INDEX] = card;

        _updateStats();
    }
    auto Board::setRiver(const Card& card) -> void {
        constexpr int8_t RIVER_CARD_INDEX = 4;

        _cards[RIVER_CARD_INDEX] = card;

        _updateStats();
    }

    auto Board::getHighCardRank() -> Card::Rank {
        return max_element(_cards, [](const Card& A, const Card& B) { return A.getRank() < B.getRank(); })->getRank();
    }

    auto Board::getHandRank(const Hand& hand) -> HandRank {
        HandRank rank            = HIGH_CARD;
        auto     rankFrequencies = _computeRankFrequencies(hand);
        auto     suitFrequencies = _computeSuitFrequencies(hand);
        bool     straight        = _straight || _countPossibleStraights(0, rankFrequencies) >= 1;

        if (_pair || count(rankFrequencies, 2) == 1) { rank = PAIR; }
        if (_twoPair || count(rankFrequencies, 2) == 2) { rank = TWO_PAIR; }
        if (_trips || count(rankFrequencies, 3) == 1) { rank = TRIPS; }
        if (straight) { rank = STRAIGHT; }
        if (_flush || count(suitFrequencies, FLUSH_SIZE) >= 1) { rank = FLUSH; }
        if (_full || (rank == TRIPS && count(rankFrequencies, 2) == 1)) { rank = FULL; }
        if (_quads || count(rankFrequencies, 4) == 1) { rank = QUADS; }
        if (_straightFlush || (rank == FLUSH && straight)) { rank = STRAIGHT_FLUSH; }

        return rank;
    }

    auto Board::toJson() const -> json {
        auto cardsArray = json::array();

        for_each(_cards, [&cardsArray](const Card& card) {
            if (!card.isUnknown()) { cardsArray.emplace_back(card.toJson()); }
        });

        return cardsArray;
    }

    auto Board::toDetailedJson() const -> json {
        auto cardsArray = json::array();

        for_each(_cards, [&cardsArray](const Card& card) {
            if (!card.isUnknown()) { cardsArray.emplace_back(card.toJson()); }
        });

        return {{"cards", cardsArray},
                {"properties",
                 {{"possibleStraight", _possibleStraight},
                  {"possibleFlush", _possibleFlush},
                  {"possibleFlushDraw", _possibleFlushDraw},
                  {"paire", _pair},
                  {"doublePaire", _twoPair},
                  {"trips", _trips},
                  {"straight", _straight},
                  {"flush", _flush},
                  {"full", _full},
                  {"quads", _quads},
                  {"straightFlush", _straightFlush}}}};
    }

    auto Board::_computeRankFrequencies(std::optional<Hand> hand) -> rank_frequencies_t {
        rank_frequencies_t frequences{};

        std::array<Card, BOARD_CARDS_NUMBER + HAND_CARDS_NUMBER> cards;

        copy(_cards.begin(), _cards.end(), cards.begin());

        if (hand != std::nullopt)
        {
            const auto& handCards = hand.value().getCards();

            copy(handCards.begin(), handCards.end(), cards.begin() + BOARD_CARDS_NUMBER);
        }

        // Construct cards rank frequences sequence
        for_each(cards, [&frequences](const Card& card) {
            if (card.getRank() != UNDEFINED) { frequences.at(static_cast<int8_t>(card.getRank()))++; }
        });

        return frequences;
    }

    auto Board::_computeSuitFrequencies(std::optional<Hand> hand) -> suit_frequencies_t {
        suit_frequencies_t frequences{};

        std::array<Card, BOARD_CARDS_NUMBER + HAND_CARDS_NUMBER> cards;

        copy(_cards.begin(), _cards.end(), cards.begin());

        if (hand != std::nullopt)
        {
            const auto& handCards = hand.value().getCards();

            copy(handCards.begin(), handCards.end(), cards.begin() + BOARD_CARDS_NUMBER);
        }

        // Construct cards suit frequences sequence
        for_each(cards, [&frequences](const Card& card) {
            if (card.getSuit() != Card::Suit::UNKNOWN) { frequences.at(static_cast<int8_t>(card.getSuit()))++; }
        });

        return frequences;
    }

    // @todo check std::views::adjacent_transform compilers implementation status
    auto Board::_countPossibleStraights(int8_t additionalCards, std::optional<rank_frequencies_t> rankFrequenciesOpt) -> int8_t {
        int8_t              possibleStraights = 0;
        rank_frequencies_t& rankFrequencies   = rankFrequenciesOpt == std::nullopt ? _rankFrequencies : rankFrequenciesOpt.value();

        // Special Ace case
        rankFrequencies[0] = rankFrequencies[static_cast<int8_t>(ACE)];

        const int8_t WINDOW_SIZE = 5;

        for (int8_t i = 0; i < static_cast<int8_t>(rankFrequencies.size() - WINDOW_SIZE + 1); ++i)
        {
            auto window = counted(rankFrequencies.begin() + i, WINDOW_SIZE);

            if (count_if(window.begin(), window.end(), [](const auto& value) { return value >= 1; })
                >= STRAIGHT_SIZE - additionalCards)
            { possibleStraights++; }
        }

        // Reset special Ace case
        rankFrequencies[0] = 0;

        return possibleStraights;
    }

    auto Board::_hasPaire() -> bool { return count(_rankFrequencies, 2) >= 1; }
    auto Board::_hasDoublePaire() -> bool { return count(_rankFrequencies, 2) >= 2; }
    auto Board::_hasTrips() -> bool { return count(_rankFrequencies, 3) == 1; }
    auto Board::_hasStraight() -> bool { return _countPossibleStraights(0) == 1; }
    auto Board::_hasPossibleStraight() -> bool { return _straight || _countPossibleStraights(2) > 0; }
    auto Board::_hasFlush() -> bool { return count(_suitFrequencies, FLUSH_SIZE) == 1; }
    auto Board::_hasPossibleFlushDraw() -> bool { return _flush || count(_suitFrequencies, 2) >= 1; }
    auto Board::_hasPossibleFlush() -> bool { return _flush || count(_suitFrequencies, 3) == 1; }
    auto Board::_hasFull() -> bool { return count(_rankFrequencies, 2) >= 1 && count(_rankFrequencies, 3) >= 1; }
    auto Board::_hasQuads() -> bool { return count(_rankFrequencies, 4) == 1; }

    auto Board::_updateStats() -> void {
        _rankFrequencies = _computeRankFrequencies();
        _suitFrequencies = _computeSuitFrequencies();
        // The order is important, possible flush or possible straight use flush and straight values for optimisation
        _pair              = _hasPaire();
        _twoPair           = _hasDoublePaire();
        _trips             = _hasTrips();
        _straight          = _hasStraight();
        _possibleStraight  = _hasPossibleStraight();
        _flush             = _hasFlush();
        _possibleFlush     = _hasPossibleFlush();
        _possibleFlushDraw = _hasPossibleFlushDraw();
        _full              = _hasFull();
        _quads             = _hasQuads();
        _straightFlush     = _straight && _flush;
    }
}  // namespace GameHandler
