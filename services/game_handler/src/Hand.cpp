#include "game_handler/Hand.hpp"

#include <algorithm>

namespace GameHandler {
    using std::tie;
    using std::ranges::all_of;
    using std::ranges::any_of;
    using std::ranges::find;

    using enum Card::Rank;

    Hand::Hand(const Card& firstCard, const Card& secondCard) :
        _firstCard(firstCard), _secondCard(secondCard), _cards({&firstCard, &secondCard}) {
        if (firstCard == secondCard) { throw invalid_hand("The two given cards are the same (" + firstCard.getShortName() + ")"); }

        _processHand();
    }

    auto Hand::operator=(const Hand& other) -> Hand& {
        if (this != &other)
        {
            _firstCard  = other._firstCard;
            _secondCard = other._secondCard;
            _cards      = {&_firstCard, &_secondCard};
            _suited     = other._suited;
            _broadway   = other._broadway;
            _plur       = other._plur;
            _connected  = other._connected;
            _premium    = other._premium;
        }

        return *this;
    }

    auto Hand::operator=(Hand&& other) noexcept -> Hand& {
        if (this != &other)
        {
            _firstCard  = std::move(other._firstCard);
            _secondCard = std::move(other._secondCard);
            _cards      = {&_firstCard, &_secondCard};
            _suited     = other._suited;
            _broadway   = other._broadway;
            _plur       = other._plur;
            _connected  = other._connected;
            _premium    = other._premium;
        }

        return *this;
    }

    auto Hand::operator==(const Hand& rhs) const -> bool {
        return tie(_firstCard, _secondCard) == tie(rhs._firstCard, rhs._secondCard);
    }

    auto Hand::operator!=(const Hand& rhs) const -> bool { return !(rhs == *this); }

    auto Hand::toJson() const -> json {
        auto cardsArray = json::array();

        if (!_firstCard.isUnknown()) { cardsArray.emplace_back(_firstCard.toJson()); }
        if (!_secondCard.isUnknown()) { cardsArray.emplace_back(_secondCard.toJson()); }

        return cardsArray;
    }

    auto Hand::toDetailedJson() const -> json {
        auto cardsArray = json::array();

        cardsArray.emplace_back(_firstCard.toJson());
        cardsArray.emplace_back(_secondCard.toJson());

        return {{"cards", cardsArray},
                {"properties",
                 {{"suited", _suited},
                  {"aceSuited", _aceSuited},
                  {"broadway", _broadway},
                  {"plur", _plur},
                  {"connected", _connected},
                  {"premium", _premium}}}};
    }

    auto Hand::_isSuited() -> bool { return _firstCard.getSuit() == _secondCard.getSuit(); }

    auto Hand::_isAceSuited() -> bool {
        return _suited && any_of(_cards, [](const auto& card) { return card->getRank() == ACE; });
    }

    auto Hand::_isBroadway() -> bool {
        return any_of(_cards, [](const auto& card) { return card->isBroadway(); });
    }

    auto Hand::_isPlur() -> bool {
        return all_of(_cards, [](const auto& card) { return card->isBroadway(); });
    }

    auto Hand::_isConnected() -> bool {
        return std::abs(_firstCard.getRank() - _secondCard.getRank()) <= 1
            || (_firstCard.getRank() == ACE && _secondCard.getRank() == TWO)
            || (_secondCard.getRank() == ACE && _firstCard.getRank() == TWO);
    }

    auto Hand::_isPremium() -> bool {
        return find(PREMIUM, std::array<Card::Rank, 2>{_firstCard.getRank(), _secondCard.getRank()}) != PREMIUM.end();
    }

    auto Hand::_processHand() -> void {
        // Order is important, aceSuited is using suited
        _suited    = _isSuited();
        _aceSuited = _isAceSuited();
        _broadway  = _isBroadway();
        _plur      = _isPlur();
        _connected = _isConnected();
        _premium   = _isPremium();
    }
}  // namespace GameHandler