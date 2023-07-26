#pragma once

#include <game_handler/Hand.hpp>

namespace GameHandler {
    class Player {
        public:
            Player()                        = default;
            Player(const Player& other)     = default;
            Player(Player&& other) noexcept = default;
            Player(std::string name, uint8_t number, bool self = false) : _name(std::move(name)), _number(number), _self(self){};

            virtual ~Player() = default;

            auto operator=(const Player& other) -> Player& = default;
            auto operator=(Player&& other) noexcept -> Player&;
            auto operator==(const Player& other) const -> bool { return _name == other._name; }

            [[nodiscard]] auto getName() const -> std::string { return _name; }
            [[nodiscard]] auto getHand() const -> Hand { return _hand; }
            [[nodiscard]] auto getNumber() const -> uint8_t { return _number; }
            [[nodiscard]] auto getStack() const -> int32_t { return _stack; }
            [[nodiscard]] auto isDealer() const -> bool { return _dealer; }
            [[nodiscard]] auto isButton() const -> bool { return isDealer(); }  // alias
            [[nodiscard]] auto isEliminated() const -> bool { return _eliminated; }
            [[nodiscard]] auto self() const -> bool { return _self; }

            auto setHand(const Card& firstCard, const Card& secondCard) -> void { _hand = Hand(firstCard, secondCard); }
            auto setStack(int32_t stack) -> void { _stack = stack; }
            auto setDealer(bool dealer) -> void { _dealer = dealer; }
            auto takeButton() -> void { setDealer(true); }
            auto bust() -> void { _eliminated = true; }

        private:
            std::string _name;
            Hand        _hand;
            uint8_t     _number     = 0;
            int32_t     _stack      = 0;
            bool        _dealer     = false;
            bool        _self       = false;
            bool        _eliminated = false;
    };
}  // namespace GameHandler