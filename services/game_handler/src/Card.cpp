#include "Card.hpp"

#include <nlohmann/json.hpp>

namespace GameHandler {
    using std::ranges::find;

    using json = nlohmann::json;

    using enum Card::Rank;
    using enum Card::Suit;

    auto Card::operator=(Card&& other) noexcept -> Card& {
        if (this != &other)
        {
            _rank = other._rank;
            _suit = other._suit;
        }

        return *this;
    }

    auto Card::getFullName() const -> std::string {
        return (_rank == UNDEFINED || _suit == UNKNOWN) ? "N/A" : rankToString(_rank) + " of " + suitToString(_suit);
    }

    auto Card::getShortName() const -> std::string {
        return (_rank == UNDEFINED || _suit == UNKNOWN) ? "NA" : rankToShortString(_rank) + suitToString(_suit)[0];
    }

    auto Card::isBroadway() const -> bool { return find(BROADWAY, _rank) != BROADWAY.end(); }

    auto Card::toJson() const -> json {
        return {{"shortName", getShortName()}, {"rank", rankToString(_rank)}, {"suit", suitToString(_suit)}};
    }
}  // namespace GameHandler