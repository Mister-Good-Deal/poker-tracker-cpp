#pragma once

#include <array>
#include <string>

#include "Card.hpp"

class Player {
    public:
        Player(std::string name);
        inline std::string         getName() const { return name; }
        inline void                setName(std::string newName) { name = newName; }
        inline std::array<Card, 2> getHand() const { return hand; }
        inline void                setHand(std::array<Card, 2> newHand) { hand = newHand; }
        inline int32_t             getStack() const { return stack; }
        inline void                setStack(int32_t newStack) { stack = newStack; }

    private:
        std::string         name;
        std::array<Card, 2> hand;
        int32_t             stack;
};
