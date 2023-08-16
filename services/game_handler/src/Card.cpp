#include "game_handler/Card.hpp"

namespace GameHandler {
    using std::ranges::find;

    using json = nlohmann::json;

    using enum Card::Rank;
    using enum Card::Suit;

    auto Card::operator=(Card&& other) noexcept -> Card& {
        if (this != &other) {
            _rank = other._rank;
            _suit = other._suit;
        }

        return *this;
    }

    auto Card::isBroadway() const -> bool { return find(BROADWAY, _rank) != BROADWAY.end(); }

    auto Card::toJson() const -> json {
        return {{"shortName", fmt::format("{:s}", *this)}, {"rank", fmt::format("{:l}", _rank)}, {"suit", fmt::format("{:l}", _suit)}};
    }
}  // namespace GameHandler