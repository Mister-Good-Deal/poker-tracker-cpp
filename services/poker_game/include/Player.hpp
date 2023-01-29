#pragma once

#include <array>
#include <string>

#include "Hand.hpp"

class Player {
    public:
        explicit Player(std::string name);

        [[nodiscard]] auto getName() const -> std::string { return name; }
        [[nodiscard]] auto getHand() const -> Hand { return hand; }
        [[nodiscard]] auto getStack() const -> int32_t { return stack; }

        void setName(std::string newName) { name = newName; }
        void setHand(const Card& firstCard, const Card& secondCard) { hand = Hand(firstCard, secondCard); }
        void setStack(int32_t newStack) { stack = newStack; }

    private:
        std::string name;
        Hand        hand;
        int32_t     stack = 0;
};
