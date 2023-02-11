#pragma once

#include "Card.hpp"

namespace GameHandler {
    using enum Card::Rank;

    constexpr int8_t PREMIUM_NUMBER    = 5;
    constexpr int8_t HAND_CARDS_NUMBER = 2;

    static constexpr std::array<std::array<Card::Rank, HAND_CARDS_NUMBER>, PREMIUM_NUMBER> PREMIUM = {
        {{{QUEEN, QUEEN}}, {{KING, ACE}}, {{ACE, KING}}, {{KING, KING}}, {{ACE, ACE}}}};

    class invalid_hand : public std::runtime_error {
        public:
            explicit invalid_hand(const std::string& arg) : runtime_error(arg){};
    };

    class Hand {
        public:
            using hand_cards_ref_t = std::array<const Card*, HAND_CARDS_NUMBER>;
            using hand_cards_t     = std::array<Card, HAND_CARDS_NUMBER>;

            Hand()                  = default;
            Hand(const Hand& other) = default;
            Hand(Hand&& other) noexcept { *this = std::move(other); };
            Hand(const Card& firstCard, const Card& secondCard);

            virtual ~Hand() = default;

            auto operator=(const Hand& other) -> Hand&;
            auto operator=(Hand&& other) noexcept -> Hand&;

            auto operator==(const Hand& rhs) const -> bool;
            auto operator!=(const Hand& rhs) const -> bool;

            [[nodiscard]] auto getCards() const -> const hand_cards_t& { return _cardsConst; };
            [[nodiscard]] auto isSuited() const -> bool { return _suited; };
            [[nodiscard]] auto isAceSuited() const -> bool { return _aceSuited; };
            [[nodiscard]] auto isBroadway() const -> bool { return _broadway; };
            [[nodiscard]] auto isPlur() const -> bool { return _plur; };
            [[nodiscard]] auto isConnected() const -> bool { return _connected; };
            [[nodiscard]] auto isPremium() const -> bool { return _premium; };

            [[nodiscard]] auto toJson() const -> json;
            [[nodiscard]] auto toDetailedJson() const -> json;

        private:
            Card             _firstCard;
            Card             _secondCard;
            hand_cards_ref_t _cards      = {&_firstCard, &_secondCard};  // Shortcut to use std algorithms in class
            hand_cards_t     _cardsConst = {_firstCard, _secondCard};    // Const value reference to avoid array duplication
            bool             _suited     = false;
            bool             _aceSuited  = false;
            bool             _broadway   = false;
            bool             _plur       = false;
            bool             _connected  = false;
            // @todo siblingsConnected
            bool _premium = false;

            auto _isSuited() -> bool;
            auto _isAceSuited() -> bool;
            auto _isBroadway() -> bool;
            auto _isPlur() -> bool;
            auto _isConnected() -> bool;
            auto _isPremium() -> bool;

            auto _processHand() -> void;
    };
}  // namespace GameHandler