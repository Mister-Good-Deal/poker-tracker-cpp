#pragma once

#include <game_handler/Hand.hpp>

namespace GameHandler {
    class Player {
        public:
            Player()                        = default;
            Player(const Player& other)     = default;
            Player(Player&& other) noexcept = default;
            Player(std::string_view name, uint32_t number, bool self = false) : _name(name), _number(number){};

            virtual ~Player() = default;

            auto operator=(const Player& other) -> Player& = default;
            auto operator=(Player&& other) noexcept -> Player&;
            auto operator==(const Player& other) const -> bool { return _name == other._name; }

            [[nodiscard]] auto getName() const -> std::string { return _name; }
            [[nodiscard]] auto getNumber() const -> uint32_t { return _number; }
            [[nodiscard]] auto getStack() const -> int32_t { return _stack; }
            [[nodiscard]] auto isEliminated() const -> bool { return _eliminated; }
            [[nodiscard]] auto isHero() const -> bool { return _number == 1; }  // The player with number 1 is the hero

            auto setStack(int32_t stack) -> void { _stack = stack; }
            auto updateStack(int32_t amount) -> void { _stack += amount; }
            auto bust() -> void { _eliminated = true; }

        private:
            std::string _name;
            uint32_t    _number     = 0;
            int32_t     _stack      = 0;
            bool        _eliminated = false;
    };
}  // namespace GameHandler