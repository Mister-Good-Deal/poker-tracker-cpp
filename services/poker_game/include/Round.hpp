#pragma once

#include <array>
#include <map>
#include <string>
#include <vector>

class Player;

class Round {
    public:
        enum class Street : int8_t {
            PREFLOP = 0,
            FLOP,
            TURN,
            RIVER,
            SHOWDOWN
        };

    public:
        Round() = default;

        [[nodiscard]] auto getPot() const -> int32_t { return pot; }

        void call(Player& player);
        void bet(Player& player, int32_t amount);
        void check(Player& player);
        void fold(Player& player);

    private:
        std::array<std::vector<std::string>, 5> playersActions;
        std::map<Player*, int32_t>              playersBets;
        int32_t                                 pot{0};
        Street                                  currentStreet{Street::PREFLOP};
        int32_t                                 callAmount{0};
};
