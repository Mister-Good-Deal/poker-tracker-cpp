#include "game_handler/Player.hpp"

namespace GameHandler {
    Player::Player(std::string_view name, int32_t number)
      : _name(name)
      , _number(number) {}

    auto Player::operator=(Player&& other) noexcept -> Player& {
        if (this != &other) {
            _name       = std::move(other._name);
            _number     = other._number;
            _stack      = other._stack;
            _eliminated = other._eliminated;
        }

        return *this;
    }
}  // namespace GameHandler