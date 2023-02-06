#include "Card.hpp"

#include <nlohmann/json.hpp>

namespace GameHandler {
    using json = nlohmann::json;

    auto Card::operator=(Card&& other) noexcept -> Card& {
        rank = other.rank;
        suit = other.suit;

        return *this;
    }

    auto Card::getFullName() const -> std::string { return rankToString(rank) + " of " + suitToString(suit); }

    auto Card::getShortName() const -> std::string { return rankToShortString(rank) + suitToString(suit)[0]; }

    auto Card::toJson() const -> json {
        return {
            {"shortName", getShortName()}, {"fullName", getFullName()}, {"rank", rankToString(rank)}, {"suit", suitToString(suit)}};
    }
}  // namespace GameHandler