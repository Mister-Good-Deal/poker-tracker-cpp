#pragma once

#include "Hand.hpp"

namespace GameHandler {
    class Player {
        public:
            explicit Player(std::string name, bool self = false) : _name(std::move(name)), _self(self){};

            [[nodiscard]] auto getName() const -> std::string { return _name; }
            [[nodiscard]] auto getHand() const -> Hand { return _hand; }
            [[nodiscard]] auto getStack() const -> int32_t { return _stack; }
            [[nodiscard]] auto isEliminated() const -> bool { return _eliminated; }
            [[nodiscard]] auto self() const -> bool { return _self; }

            auto setHand(const Card& firstCard, const Card& secondCard) -> void { _hand = Hand(firstCard, secondCard); }
            auto setStack(int32_t stack) -> void { _stack = stack; }
            auto bust() -> void { _eliminated = true; }

        private:
            std::string _name;
            Hand        _hand;
            int32_t     _stack      = 0;
            bool        _self       = false;
            bool        _eliminated = false;
    };
}  // namespace GameHandler