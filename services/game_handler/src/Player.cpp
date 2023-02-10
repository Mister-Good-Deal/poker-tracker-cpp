#include "Player.hpp"

namespace GameHandler {
    auto Player::operator=(Player&& other) noexcept -> Player& {
        if (this != &other)
        {
            _name       = std::move(other._name);
            _hand       = std::move(other._hand);
            _stack      = other._stack;
            _self       = other._self;
            _eliminated = other._eliminated;
        }

        return *this;
    }

    auto Player::operator=(const Player& other) -> Player& {
        if (this != &other)
        {
            _name       = other._name;
            _hand       = other._hand;
            _stack      = other._stack;
            _self       = other._self;
            _eliminated = other._eliminated;
        }

        return *this;
    }
}  // namespace GameHandler