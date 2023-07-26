#include "game_handler/Player.hpp"

namespace GameHandler {
    auto Player::operator=(Player&& other) noexcept -> Player& {
        if (this != &other)
        {
            _name       = std::move(other._name);
            _hand       = std::move(other._hand);
            _number     = other._number;
            _stack      = other._stack;
            _dealer     = other._dealer;
            _self       = other._self;
            _eliminated = other._eliminated;
        }

        return *this;
    }
}  // namespace GameHandler