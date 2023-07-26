#include "game_handler/Board.hpp"

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

    auto GameHandler::Board::compareHands(const Hand& hand1, const Hand& hand2) -> int {
        if (!hand1.isSet() && !hand2.isSet()) { throw std::invalid_argument("Both hands are not set"); }
        if (!hand1.isSet()) { return -1; }
        if (!hand2.isSet()) { return 1; }

        auto hand1RankAndBestCombo = _getHandRankAndBestCombo(hand1);
        auto hand2RankAndBestCombo = _getHandRankAndBestCombo(hand2);

        HandRank rank1 = hand1RankAndBestCombo.first;
        HandRank rank2 = hand2RankAndBestCombo.first;

        best_hand_t bestHand1 = hand1RankAndBestCombo.second;
        best_hand_t bestHand2 = hand2RankAndBestCombo.second;

        if (rank1 > rank2) { return 1; }
        if (rank1 < rank2) { return -1; }

        // Assuming that cards of the best hand are sorted by rank in descending order
        for (int i = 0; i < COMPARISON_CARDS_NUMBER; ++i)
        {
            if (bestHand1.at(i).getRank() > bestHand2.at(i).getRank()) { return 1; }
            if (bestHand1.at(i).getRank() < bestHand2.at(i).getRank()) { return -1; }
        }

        return 0;  // Both best hands are equal
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
            if (card.getRank() != UNDEFINED) { frequences.at(card.getRank())++; }
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
            if (card.getSuit() != Card::Suit::UNKNOWN) { frequences.at(card.getSuit())++; }
        });

        return frequences;
    }

    // @todo check std::views::adjacent_transform compilers implementation status
    auto Board::_countPossibleStraights(int8_t additionalCards, std::optional<rank_frequencies_t> rankFrequenciesOpt) -> int8_t {
        int8_t              possibleStraights = 0;
        rank_frequencies_t& rankFrequencies   = rankFrequenciesOpt == std::nullopt ? _rankFrequencies : rankFrequenciesOpt.value();

        // Special Ace case
        rankFrequencies[0] = rankFrequencies[ACE];

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

    auto Board::_extractComboFromStraightOrFlush(const all_cards_t& cards, HandRank rank, rank_frequencies_t& rankFrequencies,
                                                 suit_frequencies_t& suitFrequencies) -> combo_t {
        std::vector<Card> combo;
        int32_t           index = BOARD_CARDS_NUMBER - 1;

        // Special Ace case
        rankFrequencies[0] = rankFrequencies[ACE];

        auto match = [](const auto& cards) {
            return count_if(cards.begin(), cards.end(), [](const auto& value) { return value >= 1; }) >= STRAIGHT_SIZE;
        };

        auto windowStraight = counted(rankFrequencies.begin() + index, STRAIGHT_SIZE);
        auto windowFlush    = counted(suitFrequencies.begin() + index, FLUSH_SIZE);

        switch (rank)
        {
            case HandRank::STRAIGHT:
                while (!match(windowStraight))
                { windowStraight = counted(rankFrequencies.begin() + index--, STRAIGHT_SIZE); }
                break;
            case HandRank::FLUSH:
                while (!match(windowFlush))
                { windowFlush = counted(suitFrequencies.begin() + index--, FLUSH_SIZE); }
                break;
            case HandRank::STRAIGHT_FLUSH:
                while (!match(windowStraight) && !match(windowFlush))
                {
                    windowStraight = counted(rankFrequencies.begin() + index, STRAIGHT_SIZE);
                    windowFlush    = counted(suitFrequencies.begin() + index--, STRAIGHT_SIZE);
                }
                break;
            default: throw std::invalid_argument("The given hand rank is invalid");
        }

        combo = {cards[index], cards[index - 1], cards[index - 2], cards[index - 3], cards[index - 4]};
        // Reset special Ace case
        rankFrequencies[0] = 0;

        return combo;
    }

    auto Board::_extractComboFromPairsLike(const all_cards_t& cards, HandRank rank, rank_frequencies_t& rankFrequencies) -> combo_t {
        std::vector<Card> combo;

        auto extractCombo = [&](int value) {
            for (const auto& card : cards)
            {
                if (rankFrequencies[card.getRank()] == value) { combo.push_back(card); }
            }
        };

        switch (rank)
        {
            case PAIR: extractCombo(PAIR_SIZE); break;
            case TWO_PAIR:
                extractCombo(PAIR_SIZE);
                _trimCombo(TWO_PAIR, combo);
                break;
            case TRIPS:
                extractCombo(FULL_SIZE);
                _trimCombo(TRIPS, combo);
                break;
            case FULL:
                extractCombo(PAIR_SIZE);
                extractCombo(TRIPS_SIZE);
                _trimCombo(FULL, combo);
                break;
            case QUADS: extractCombo(QUADS_SIZE); break;
            default: throw std::invalid_argument("The given hand rank is invalid");
        }

        return combo;
    }

    auto Board::_trimCombo(HandRank rank, combo_t& combo) -> void {
        // Order cards
        std::ranges::sort(combo, [](const Card& A, const Card& B) { return A.getRank() > B.getRank(); });
        // Trims the combo to the correct size
        switch (rank)
        {
            case HandRank::TWO_PAIR: combo.erase(combo.begin() + TWO_PAIR_SIZE, combo.end()); break;
            case HandRank::TRIPS: combo.erase(combo.begin() + TRIPS_SIZE, combo.end()); break;
            case HandRank::FULL: combo.erase(combo.begin() + FULL_SIZE, combo.end()); break;
            default: throw std::invalid_argument("The given hand rank is invalid");
        }
    }

    auto Board::_extractCombo(const all_cards_t& cards, HandRank rank, rank_frequencies_t& rankF, suit_frequencies_t& suitF)
        -> combo_t {
        switch (rank)
        {
            case HandRank::STRAIGHT:
            case HandRank::FLUSH:
            case HandRank::STRAIGHT_FLUSH: return _extractComboFromStraightOrFlush(cards, rank, rankF, suitF);
            case HandRank::PAIR:
            case HandRank::TWO_PAIR:
            case HandRank::TRIPS:
            case HandRank::FULL:
            case HandRank::QUADS: return _extractComboFromPairsLike(cards, rank, rankF);
            default: throw std::invalid_argument("The given hand rank is invalid");
        }
    }

    // Find the highest cards that are not in the combo
    auto Board::_extractHigherCards(all_cards_t& cards, const combo_t& combo) -> best_hand_t {
        best_hand_t higherCards;
        auto        currentSize      = combo.size();
        int         highestCardIndex = 0;

        // Fill the higher cards array with the combo cards
        std::copy(combo.begin(), combo.end(), higherCards.begin());
        // Sort the cards by rank in descending order
        std::ranges::sort(cards, [](const Card& A, const Card& B) { return A.getRank() > B.getRank(); });

        while (currentSize < COMPARISON_CARDS_NUMBER)
        {
            auto& highestCard = cards[highestCardIndex++];
            // If the card is not in the combo, add it to the higher cards array
            if (std::find(combo.begin(), combo.end(), highestCard) == combo.end()) { higherCards[currentSize++] = highestCard; }
        }

        // Sort the higher cards by rank in descending order
        std::ranges::sort(higherCards, [](const Card& A, const Card& B) { return A.getRank() > B.getRank(); });

        return higherCards;
    }

    // Similar to getHandRank()
    auto Board::_getHandRankAndBestCombo(const Hand& hand) -> std::pair<HandRank, best_hand_t> {
        HandRank          rank            = HIGH_CARD;
        std::vector<Card> combo           = {};
        auto              rankFrequencies = _computeRankFrequencies(hand);
        auto              suitFrequencies = _computeSuitFrequencies(hand);
        bool              straight        = _countPossibleStraights(0, rankFrequencies) >= 1;

        // Determine the hand rank
        if (count(rankFrequencies, 2) == 1) { rank = PAIR; }
        if (count(rankFrequencies, 2) >= 2) { rank = TWO_PAIR; }
        if (count(rankFrequencies, 3) == 1) { rank = TRIPS; }
        if (straight) { rank = STRAIGHT; }
        if (count(suitFrequencies, FLUSH_SIZE) >= 1) { rank = FLUSH; }
        if (count(rankFrequencies, 3) == 2 || (count(rankFrequencies, 3) == 1 && count(rankFrequencies, 2) >= 1)) { rank = FULL; }
        if (count(rankFrequencies, 4) == 1) { rank = QUADS; }
        if (rank == FLUSH && straight) { rank = STRAIGHT_FLUSH; }

        // Combine board cards with hand cards
        all_cards_t cards;
        std::copy(_cards.begin(), _cards.end(), cards.begin());
        const auto& handCards = hand.getCards();
        std::copy(handCards.begin(), handCards.end(), cards.begin() + BOARD_CARDS_NUMBER);

        return {rank, _extractHigherCards(cards, _extractCombo(cards, rank, rankFrequencies, suitFrequencies))};
    }
}  // namespace GameHandler
