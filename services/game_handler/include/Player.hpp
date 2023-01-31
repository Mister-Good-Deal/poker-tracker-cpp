#pragma once

#include <array>
#include <string>
#include <utility>

#include "Hand.hpp"

namespace GameHandler {
    class Player {
        public:
            explicit Player(std::string name) : name(std::move(name)){};

            [[nodiscard]] auto getName() const -> std::string { return name; }
            [[nodiscard]] auto getHand() const -> Hand { return hand; }
            [[nodiscard]] auto getStack() const -> int32_t { return stack; }
            [[nodiscard]] auto isEliminated() const -> bool { return eliminated; }

            auto setHand(const Card& firstCard, const Card& secondCard) -> void { hand = Hand(firstCard, secondCard); }
            auto setStack(int32_t newStack) -> void { stack = newStack; }
            auto bust() -> void { eliminated = true; }

        private:
            std::string name;
            Hand        hand;
            int32_t     stack      = 0;
            bool        eliminated = false;
    };
}  // namespace GameHandler