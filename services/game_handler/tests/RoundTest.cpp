#include <gtest/gtest.h>

#include "CardFactory.hpp"
#include "Round.hpp"

using GameHandler::Board;
using GameHandler::Player;
using GameHandler::Round;
using GameHandler::seconds;
using GameHandler::Factory::card;

class RoundTest : public ::testing::Test {};

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
TEST(RoundTest, jsonRepresentationShouldBeCorrect) {
    Round  round;
    Player player1("player_1", true);
    Player player2("player_2");
    Player player3("player_3");

    // Run a scenario

    // Pre-flop
    round.start();
    player1.setHand(card("AH"), card("KH"));
    round.setHand(player1.getHand());
    std::this_thread::sleep_for(seconds(2));
    round.check(player1);
    std::this_thread::sleep_for(seconds(1));
    round.bet(player2, 100);
    std::this_thread::sleep_for(seconds(3));
    round.fold(player3);
    round.call(player1, 100);
    round.endStreet();
    // Flop
    round.getBoard().setFlop({card("AS"), card("AC"), card("3C")});
    std::this_thread::sleep_for(seconds(3));
    round.check(player1);
    std::this_thread::sleep_for(seconds(1));
    round.bet(player2, 200);
    std::this_thread::sleep_for(seconds(1));
    round.bet(player1, 600);
    std::this_thread::sleep_for(seconds(4));
    round.fold(player2);
    round.end(player1);
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
            "won": true,
            "winner": "player_1"
        }
    )"_json;

    EXPECT_EQ(round.toJson(), expectedJson);
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
