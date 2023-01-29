#include "Player.hpp"

#include <utility>

Player::Player(std::string name) : name(std::move(name)) {}
