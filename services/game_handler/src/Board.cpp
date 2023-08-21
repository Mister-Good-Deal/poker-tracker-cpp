#include "game_handler/Board.hpp"

namespace GameHandler {
    using std::ranges::all_of;
    using std::ranges::any_of;
    using std::ranges::copy;
    using std::ranges::count;
    using std::ranges::count_if;
    using std::ranges::distance;
    using std::ranges::find;
    using std::ranges::find_if;
    using std::ranges::for_each;
    using std::ranges::max_element;
    using std::ranges::sort;
    using std::views::counted;

    using enum Card::Rank;
    using enum Card::Suit;
    using enum HandRank;

    auto Board::operator=(Board&& other) noexcept -> Board& {
        if (this != &other) {
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
        copy(cards, _cards.begin());

        _updateStats();
    }

    auto Board::setTurn(const Card& card) -> void {
        constexpr int32_t TURN_CARD_INDEX = 3;

        _cards[TURN_CARD_INDEX] = card;

        _updateStats();
    }
    auto Board::setRiver(const Card& card) -> void {
        constexpr int32_t RIVER_CARD_INDEX = 4;

        _cards[RIVER_CARD_INDEX] = card;

        _updateStats();
    }

    auto Board::isFlopEmpty() const -> bool {
        return any_of(getFlop(), [](const Card& card) { return card.isUnknown(); });
    }

    auto Board::getHighCardRank() -> Card::Rank {
        return max_element(_cards, [](const Card& A, const Card& B) { return A.getRank() < B.getRank(); })->getRank();
    }

    auto Board::getHandRank(const Hand& hand) -> HandRank {
        HandRank rank     = HIGH_CARD;
        auto     rankF    = _computeRankFrequencies(hand);
        auto     suitF    = _computeSuitFrequencies(hand);
        bool     straight = _straight || _countPossibleStraights(0, rankF) >= 1;

        // Combine board cards with hand cards
        all_cards_t cards;
        copy(_cards, cards.begin());
        const auto& handCards = hand.getCards();
        copy(handCards, cards.begin() + BOARD_CARDS_NUMBER);

        if (_pair || count(rankF, 2) == 1) { rank = PAIR; }
        if (_twoPair || count(rankF, 2) == 2) { rank = TWO_PAIR; }
        if (_trips || count(rankF, 3) == 1) { rank = TRIPS; }
        if (straight) { rank = STRAIGHT; }
        if (_flush || any_of(suitF, [](const auto& value) { return value >= FLUSH_SIZE; })) { rank = FLUSH; }
        if (_full || (count(rankF, 3) == 2 || (count(rankF, 3) == 1 && count(rankF, 2) >= 1))) { rank = FULL; }
        if (_quads || count(rankF, 4) == 1) { rank = QUADS; }
        if (_straightFlush || (rank == FLUSH && straight && _isStraightFlush(cards, rankF, suitF))) { rank = STRAIGHT_FLUSH; }

        return rank;
    }

    auto Board::compareHands(const Hand& hand1, const Hand& hand2) -> int {
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
        for (int i = 0; i < COMPARISON_CARDS_NUMBER; ++i) {
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

    auto Board::_computeRankFrequencies(std::optional<Hand> hand) -> rank_f_t {
        rank_f_t frequences{};

        all_cards_t cards;

        copy(_cards, cards.begin());

        if (hand != std::nullopt) {
            const auto& handCards = hand.value().getCards();

            copy(handCards, cards.begin() + BOARD_CARDS_NUMBER);
        }

        // Construct cards rank frequences sequence
        for_each(cards, [&frequences](const Card& card) {
            if (card.getRank() != UNDEFINED) { frequences.at(card.getRank())++; }
        });

        return frequences;
    }

    auto Board::_computeSuitFrequencies(std::optional<Hand> hand) -> suit_f_t {
        suit_f_t frequences{};

        std::array<Card, BOARD_CARDS_NUMBER + HAND_CARDS_NUMBER> cards;

        copy(_cards, cards.begin());

        if (hand != std::nullopt) {
            const auto& handCards = hand.value().getCards();

            copy(handCards, cards.begin() + BOARD_CARDS_NUMBER);
        }

        // Construct cards suit frequences sequence
        for_each(cards, [&frequences](const Card& card) {
            if (card.getSuit() != Card::Suit::UNKNOWN) { frequences.at(card.getSuit())++; }
        });

        return frequences;
    }

    // @todo check std::views::adjacent_transform compilers implementation status
    auto Board::_countPossibleStraights(int32_t additionalCards, std::optional<rank_f_t> rankFrequenciesOpt) -> int32_t {
        int32_t   possibleStraights = 0;
        rank_f_t& rankFrequencies   = rankFrequenciesOpt == std::nullopt ? _rankFrequencies : rankFrequenciesOpt.value();

        // Special Ace case
        rankFrequencies[0] = rankFrequencies[ACE];

        const int32_t WINDOW_SIZE = 5;

        for (int32_t i = 0; i < static_cast<int32_t>(rankFrequencies.size() - WINDOW_SIZE + 1); ++i) {
            auto window = counted(rankFrequencies.begin() + i, WINDOW_SIZE);

            if (count_if(window, [](const auto& value) { return value >= 1; }) >= STRAIGHT_SIZE - additionalCards) {
                possibleStraights++;
            }
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

    auto Board::_isStraightFlush(const all_cards_t& cards, rank_f_t& rankF, suit_f_t& suitF) -> bool {
        auto suit = static_cast<Card::Suit>(distance(suitF.begin(), find_if(suitF, [](int32_t val) { return val >= STRAIGHT_SIZE; })));

        auto extractCards = [&](int32_t index, std::vector<Card>& extractedCards) {
            for (const auto& card : cards) {
                if (card.getRank() == index) { extractedCards.emplace_back(card); }
            }
        };

        // Special Ace case
        rankF[0] = rankF[ACE];

        for (int32_t i = 0; i < static_cast<int32_t>(rankF.size() - STRAIGHT_SIZE + 1); ++i) {
            auto window = counted(rankF.begin() + i, STRAIGHT_SIZE);

            if (count_if(window, [](const auto& value) { return value >= 1; }) >= STRAIGHT_SIZE) {
                bool    isStraightFlush = true;
                int32_t j               = 0;

                while (isStraightFlush && j < STRAIGHT_SIZE) {
                    std::vector<Card> straightCards;

                    extractCards(i + j++, straightCards);

                    if (all_of(straightCards, [&](const auto& card) { return card.getSuit() != suit; })) { isStraightFlush = false; }
                }

                if (isStraightFlush) { return true; }
            }
        }

        // Reset special Ace case
        rankF[0] = 0;

        return false;
    }

    auto Board::_extractComboFromStraightOrFlush(const all_cards_t& cards, HandRank rank, rank_f_t& rankF, suit_f_t& suitF)
        -> combo_t {
        std::vector<Card> combo;
        int32_t           index               = STRAIGHT_SIZE;
        auto              rankFrequenciesSize = static_cast<int32_t>(rankF.size());
        Card::Suit        suit                = Card::Suit::UNKNOWN;

        // Special Ace case
        rankF[0] = rankF[ACE];

        auto isRankEndIndex = [](const auto& window) {
            return count_if(window, [](const auto& value) { return value >= 1; }) == STRAIGHT_SIZE;
        };

        auto flushSuitIndex = [](const suit_f_t& frequencies) {
            return distance(frequencies.begin(), find_if(frequencies, [](int32_t val) { return val >= STRAIGHT_SIZE; }));
        };

        auto inStraightRange = [&](const Card& card, int32_t startRank) {
            return card.getRank() >= startRank && card.getRank() <= startRank + STRAIGHT_SIZE;
        };

        auto window = counted(rankF.end() - index, STRAIGHT_SIZE);

        switch (rank) {
            case STRAIGHT:
                while (!isRankEndIndex(window)) { window = counted(rankF.end() - ++index, STRAIGHT_SIZE); }

                for (const auto& card : cards) {
                    if (inStraightRange(card, rankFrequenciesSize - index)) { combo.push_back(card); }
                }
                break;
            case FLUSH:
                suit = static_cast<Card::Suit>(flushSuitIndex(suitF));

                for (const auto& card : cards) {
                    if (card.getSuit() == suit) { combo.push_back(card); }
                }
                break;
            case STRAIGHT_FLUSH:
                while (!isRankEndIndex(window)) { window = counted(rankF.end() - ++index, STRAIGHT_SIZE); }
                suit = static_cast<Card::Suit>(flushSuitIndex(suitF));

                for (const auto& card : cards) {
                    if (inStraightRange(card, rankFrequenciesSize - index) && card.getSuit() == suit) { combo.push_back(card); }
                }
                break;
            default: throw std::invalid_argument("The given hand rank is invalid");
        }

        return combo;
    }

    auto Board::_extractComboFromPairsLike(const all_cards_t& cards, HandRank rank, rank_f_t& rankFrequencies) -> combo_t {
        combo_t combo;

        auto extractCombo = [&](int value) {
            for (const auto& card : cards) {
                if (rankFrequencies[card.getRank()] == value) { combo.push_back(card); }
            }
        };

        switch (rank) {
            case HIGH_CARD: break;
            case PAIR: extractCombo(PAIR_SIZE); break;
            case TWO_PAIR:
                extractCombo(PAIR_SIZE);
                _trimCombo(TWO_PAIR, combo);
                break;
            case TRIPS:
                extractCombo(TRIPS_SIZE);
                _trimCombo(TRIPS, combo);
                break;
            case FULL:
                extractCombo(TRIPS_SIZE);
                extractCombo(PAIR_SIZE);
                _trimCombo(FULL, combo);
                break;
            case QUADS: extractCombo(QUADS_SIZE); break;
            default: throw std::invalid_argument("The given hand rank is invalid");
        }

        return combo;
    }

    auto Board::_trimCombo(HandRank rank, combo_t& combo) -> void {
        // Order cards
        sort(combo, [](const Card& A, const Card& B) { return A.getRank() > B.getRank(); });
        // Trims the combo to the correct size
        switch (rank) {
            case TWO_PAIR: combo.erase(combo.begin() + TWO_PAIR_SIZE, combo.end()); break;
            case TRIPS: combo.erase(combo.begin() + TRIPS_SIZE, combo.end()); break;
            case FULL: combo.erase(combo.begin() + FULL_SIZE, combo.end()); break;
            default: throw std::invalid_argument("The given hand rank is invalid");
        }
    }

    auto Board::_extractCombo(const all_cards_t& cards, HandRank rank, rank_f_t& rankF, suit_f_t& suitF) -> combo_t {
        switch (rank) {
            case STRAIGHT:
            case FLUSH:
            case STRAIGHT_FLUSH: return _extractComboFromStraightOrFlush(cards, rank, rankF, suitF);
            case HIGH_CARD:
            case PAIR:
            case TWO_PAIR:
            case TRIPS:
            case FULL:
            case QUADS: return _extractComboFromPairsLike(cards, rank, rankF);
            default: throw std::invalid_argument("The given hand rank is invalid");
        }
    }

    // Find the highest cards that are not in the combo
    auto Board::_extractHigherCards(all_cards_t& cards, HandRank rank, const combo_t& combo) -> best_hand_t {
        best_hand_t higherCards;
        auto        currentSize      = combo.size();
        int         highestCardIndex = 0;

        // Fill the higher cards array with the combo cards
        copy(combo, higherCards.begin());
        // Sort the all the cards by rank in descending order
        sort(cards, [](const Card& A, const Card& B) { return A.getRank() > B.getRank(); });
        // Sort the higher cards by rank in descending order for needed hand ranks
        if (rank == TWO_PAIR || rank == STRAIGHT || rank == FLUSH || rank == FULL || rank == STRAIGHT_FLUSH) {
            sort(higherCards, [](const Card& A, const Card& B) { return A.getRank() > B.getRank(); });
            // Put trips cards in the first 3 positions
            if (rank == FULL && count(combo, higherCards.front()) != TRIPS_SIZE) {
                std::swap(higherCards[0], higherCards[4]);
                std::swap(higherCards[1], higherCards[3]);
            }
        }
        // Add additional higher rank cards to the higher cards array
        while (currentSize < COMPARISON_CARDS_NUMBER) {
            auto& highestCard = cards[highestCardIndex++];
            // If the card is not in the combo, add it to the higher cards array
            if (find(combo, highestCard) == combo.end()) { higherCards[currentSize++] = highestCard; }
        }

        return higherCards;
    }

    // Similar to getHandRank()
    auto Board::_getHandRankAndBestCombo(const Hand& hand) -> std::pair<HandRank, best_hand_t> {
        HandRank          rank            = HIGH_CARD;
        std::vector<Card> combo           = {};
        auto              rankFrequencies = _computeRankFrequencies(hand);
        auto              suitFrequencies = _computeSuitFrequencies(hand);
        bool              straight        = _countPossibleStraights(0, rankFrequencies) >= 1;

        // Combine board cards with hand cards
        all_cards_t cards;
        copy(_cards, cards.begin());
        copy(hand.getCards(), cards.begin() + BOARD_CARDS_NUMBER);

        // Determine the hand rank
        if (count(rankFrequencies, 2) == 1) { rank = PAIR; }
        if (count(rankFrequencies, 2) >= 2) { rank = TWO_PAIR; }
        if (count(rankFrequencies, 3) == 1) { rank = TRIPS; }
        if (straight) { rank = STRAIGHT; }
        if (any_of(suitFrequencies, [](const auto& value) { return value >= FLUSH_SIZE; })) { rank = FLUSH; }
        if (count(rankFrequencies, 3) == 2 || (count(rankFrequencies, 3) == 1 && count(rankFrequencies, 2) >= 1)) { rank = FULL; }
        if (count(rankFrequencies, 4) == 1) { rank = QUADS; }
        if (rank == FLUSH && straight && _isStraightFlush(cards, rankFrequencies, suitFrequencies)) { rank = STRAIGHT_FLUSH; }

        return {rank, _extractHigherCards(cards, rank, _extractCombo(cards, rank, rankFrequencies, suitFrequencies))};
    }
}  // namespace GameHandler
