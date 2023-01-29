#pragma once

#include <array>
#include <string>

#include "Card.hpp"

class Player {
    public:
        explicit Player(std::string name);
        [[nodiscard]] inline auto  getName() const -> std::string { return name; }
        inline void                setName(std::string newName) { name = newName; }
        [[nodiscard]] inline auto  getHand() const -> std::array<Card, 2> { return hand; }
        inline void                setHand(std::array<Card, 2> newHand) { hand = newHand; }
        [[nodiscard]] inline auto  getStack() const -> int32_t { return stack; }
        inline void                setStack(int32_t newStack) { stack = newStack; }

    private:
        std::string         name;
        std::array<Card, 2> hand;
        int32_t             stack;
};
