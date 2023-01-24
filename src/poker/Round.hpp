#ifndef ROUND_HPP
#define ROUND_HPP

#include <array>
#include <string>
#include <vector>
#include <map>

class Player;

enum class Street : int
{
    PREFLOP = 0,
    FLOP,
    TURN,
    RIVER,
    SHOWDOWN
};

class Round
{
    private:
        std::array<std::vector<std::string>,5> playersActions;
        std::map<Player*,int32_t> playersBets;
        int32_t Pot;
        Street currentStreet;
        int32_t callAmount;
    public:
        Round();
        void call(Player& player);
        void bet(Player& player, int32_t amount);
        void check(Player& player);
        void fold(Player& player);
        int32_t getPot();
};

#endif // ROUND_HPP
