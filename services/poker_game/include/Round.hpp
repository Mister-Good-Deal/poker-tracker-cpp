#pragma once

#include <array>
#include <map>
#include <string>
#include <vector>

class Player;

class Round {
        enum class Street : int {
            PREFLOP = 0,
            FLOP,
            TURN,
            RIVER,
            SHOWDOWN
        };

    public:
        Round();

        [[nodiscard]] auto getPot() const -> int32_t { return Pot; }

        void call(Player& player);
        void bet(Player& player, int32_t amount);
        void check(Player& player);
        void fold(Player& player);

    private:
        std::array<std::vector<std::string>, 5> playersActions;
        std::map<Player*, int32_t>              playersBets;
        int32_t                                 Pot;
        Street                                  currentStreet;
        int32_t                                 callAmount;
};
