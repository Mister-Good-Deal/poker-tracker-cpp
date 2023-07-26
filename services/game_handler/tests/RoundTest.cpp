#include <gtest/gtest.h>

#include <game_handler/CardFactory.hpp>
#include <game_handler/Round.hpp>

using GameHandler::Blinds;
using GameHandler::Board;
using GameHandler::Hand;
using GameHandler::Player;
using GameHandler::Round;
using GameHandler::seconds;
using GameHandler::Factory::card;

class RoundTest : public ::testing::Test {};

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
TEST(RoundTest, jsonRepresentationShouldBeCorrect) {
    auto blinds = Blinds{50, 100};
    auto hand   = Hand(card("AH"), card("KH"));

    Player player1("player_1", 1);
    Player player2("player_2", 2);
    Player player3("player_3", 3);

    std::array<Player, 3> players = {player1, player2, player3};

    Round round(hand, blinds, players);

    // Run a scenario

    // Pre-flop
    std::this_thread::sleep_for(seconds(2));
    round.check(1);
    std::this_thread::sleep_for(seconds(1));
    round.bet(2, 100);
    std::this_thread::sleep_for(seconds(3));
    round.fold(3);
    round.call(1, 100);
    // Flop
    round.getBoard().setFlop({card("AS"), card("AC"), card("3C")});
    std::this_thread::sleep_for(seconds(3));
    round.check(1);
    std::this_thread::sleep_for(seconds(1));
    round.bet(2, 200);
    std::this_thread::sleep_for(seconds(1));
    round.bet(1, 600);
    std::this_thread::sleep_for(seconds(4));
    round.fold(2);

    // language=json
    auto expectedJson = R"(
        {
            "actions": {
                "pre_flop": [
                    { "action": "Check", "player_name": "player_1", "elapsed_time": 2 },
                    { "action": "Bet", "player_name": "player_2", "elapsed_time": 1, "amount": 100 },
                    { "action": "Fold", "player_name": "player_3", "elapsed_time": 3 },
                    { "action": "Call", "player_name": "player_1", "elapsed_time": 0, "amount": 100 }
                ],
                "flop": [
                    { "action": "Check", "player_name": "player_1", "elapsed_time": 3 },
                    { "action": "Bet", "player_name": "player_2", "elapsed_time": 1, "amount": 200 },
                    { "action": "Bet", "player_name": "player_1", "elapsed_time": 1, "amount": 600 },
                    { "action": "Fold", "player_name": "player_2", "elapsed_time": 4 }
                ],
                "turn": [],
                "river": []
            },
            "board": [
                { "shortName": "AS", "rank": "Ace", "suit": "Spade" },
                { "shortName": "AC", "rank": "Ace", "suit": "Club" },
                { "shortName": "3C", "rank": "Three", "suit": "Club" }
            ],
            "hand": [
                { "shortName": "AH", "rank": "Ace", "suit": "Heart" },
                { "shortName": "KH", "rank": "King", "suit": "Heart" }
            ],
            "pot": 1000,
            "bet": 700,
            "blinds": { "small": 50, "big": 100 },
            "won": true,
            "winner": "player_1"
        }
    )"_json;

    EXPECT_EQ(round.toJson(), expectedJson);
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
