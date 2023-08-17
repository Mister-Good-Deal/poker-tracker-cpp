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
// @todo Run the test in background to not slow down the CI
TEST(RoundTest, DISABLED_jsonRepresentationWithTimingsShouldBeCorrect) {
    auto blinds = Blinds{50, 100};
    auto hand   = Hand(card("AH"), card("KH"));

    Player player1("player_1", 1);
    Player player2("player_2", 2);
    Player player3("player_3", 3);

    player1.setStack(1000);
    player2.setStack(1000);
    player3.setStack(1000);

    std::array<Player, 3> players = {player1, player2, player3};

    Round round(blinds, players, hand, 1);

    // Run a scenario

    // Pre-flop
    std::this_thread::sleep_for(seconds(2));
    round.check(1);
    std::this_thread::sleep_for(seconds(1));
    round.bet(2, 150);
    std::this_thread::sleep_for(seconds(3));
    round.fold(3);
    round.call(1, 200);
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
                    { "action": "Check", "player": "player_1", "elapsed_time": 2 },
                    { "action": "Bet", "player": "player_2", "elapsed_time": 1, "amount": 150 },
                    { "action": "Fold", "player": "player_3", "elapsed_time": 3 },
                    { "action": "Call", "player": "player_1", "elapsed_time": 0, "amount": 200 }
                ],
                "flop": [
                    { "action": "Check", "player": "player_1", "elapsed_time": 3 },
                    { "action": "Bet", "player": "player_2", "elapsed_time": 1, "amount": 200 },
                    { "action": "Bet", "player": "player_1", "elapsed_time": 1, "amount": 600 },
                    { "action": "Fold", "player": "player_2", "elapsed_time": 4 }
                ],
                "turn": [],
                "river": []
            },
            "board": [
                { "shortName": "AS", "rank": "Ace", "suit": "Spade" },
                { "shortName": "AC", "rank": "Ace", "suit": "Club" },
                { "shortName": "3C", "rank": "Three", "suit": "Club" }
            ],
            "hands": {
                "player_1": [
                    { "shortName": "AH", "rank": "Ace", "suit": "Heart" },
                    { "shortName": "KH", "rank": "King", "suit": "Heart" }
                ],
                "player_2": [],
                "player_3": []
            },
            "pot": 1300,            
            "blinds": { "small": 50, "big": 100 },
            "won": true,
            "ranking": [["player_1"], ["player_2"], ["player_3"]],
            "positions": {
                "dealer": "player_1",
                "small_blind": "player_2",
                "big_blind": "player_3"
            },
            "stacks": [
                { "player": "player_1", "stack": 1500, "balance": 500 },
                { "player": "player_2", "stack": 600, "balance": -400 },
                { "player": "player_3", "stack": 900, "balance": -100 }
            ]
        }
    )"_json;

    EXPECT_EQ(round.toJson(), expectedJson);
}

TEST(RoundTest, case1JsonRepresentationShouldBeCorrect) {
    Player player1("player 1", 1);
    Player player2("player 2", 2);
    Player player3("player 3", 3);

    player1.setStack(1000);
    player2.setStack(1000);
    player3.setStack(1000);

    std::array<Player, 3> players = {player1, player2, player3};

    Round round({50, 100}, players, {card("AH"), card("KH")}, 1);

    // Run a scenario

    // Pre-flop
    round.check(1);
    round.raise(2, 150);
    round.fold(3);
    round.call(1, 200);
    // Flop
    round.getBoard().setFlop({card("AS"), card("AC"), card("3C")});
    round.check(1);
    round.raise(2, 200);
    round.raise(1, 600);
    round.fold(2);

    // language=json
    auto expectedJson = R"(
        {
            "actions": {
                "pre_flop": [
                    { "action": "Check", "player": "player_1", "elapsed_time": 0 },
                    { "action": "Raise", "player": "player_2", "elapsed_time": 0, "amount": 150 },
                    { "action": "Fold", "player": "player_3", "elapsed_time": 0 },
                    { "action": "Call", "player": "player_1", "elapsed_time": 0, "amount": 200 }
                ],
                "flop": [
                    { "action": "Check", "player": "player_1", "elapsed_time": 0 },
                    { "action": "Raise", "player": "player_2", "elapsed_time": 0, "amount": 200 },
                    { "action": "Raise", "player": "player_1", "elapsed_time": 0, "amount": 600 },
                    { "action": "Fold", "player": "player_2", "elapsed_time": 0 }
                ],
                "turn": [],
                "river": []
            },
            "board": [
                { "shortName": "AS", "rank": "Ace", "suit": "Spade" },
                { "shortName": "AC", "rank": "Ace", "suit": "Club" },
                { "shortName": "3C", "rank": "Three", "suit": "Club" }
            ],
            "hands": {
                "player_1": [
                    { "shortName": "AH", "rank": "Ace", "suit": "Heart" },
                    { "shortName": "KH", "rank": "King", "suit": "Heart" }
                ],
                "player_2": [],
                "player_3": []
            },
            "pot": 1300,            
            "blinds": { "small": 50, "big": 100 },
            "won": true,
            "positions": {
                "dealer": "player_1",
                "small_blind": "player_2",
                "big_blind": "player_3"
            },
            "ranking": [["player_1"], ["player_2"], ["player_3"]],
            "stacks": [
                { "player": "player_1", "stack": 1500, "balance": 500 },
                { "player": "player_2", "stack": 600, "balance": -400 },
                { "player": "player_3", "stack": 900, "balance": -100 }
            ]
        }
    )"_json;

    EXPECT_EQ(round.toJson(), expectedJson);
}

TEST(RoundTest, case2JsonRepresentationShouldBeCorrect) {
    Player player1("player_1", 1);
    Player player2("player_2", 2);
    Player player3("player_3", 3);

    player1.setStack(1000);
    player2.setStack(1000);
    player3.setStack(1000);

    std::array<Player, 3> players = {player1, player2, player3};

    Round round(Blinds{50, 100}, players, {card("AH"), card("KH")}, 1);

    // Run a scenario

    // Pre-flop
    round.check(1);
    round.raise(2, 150);
    round.fold(3);
    round.call(1, 200);
    // Flop
    round.getBoard().setFlop({card("AS"), card("AC"), card("3C")});
    round.check(1);
    round.raise(2, 200);
    round.raise(1, 600);
    round.call(2, 400);
    // Turn
    round.getBoard().setTurn(card("4C"));
    round.check(1);
    round.check(2);
    // River
    round.getBoard().setRiver(card("8D"));
    round.check(1);
    round.check(2);
    // Showdown
    round.setPlayer2Hand({card("AD"), card("KC")});
    round.showdown();

    // language=json
    auto expectedJson = R"(
        {
            "actions": {
                "pre_flop": [
                    { "action": "Check", "player": "player_1", "elapsed_time": 0 },
                    { "action": "Raise", "player": "player_2", "elapsed_time": 0, "amount": 150 },
                    { "action": "Fold", "player": "player_3", "elapsed_time": 0 },
                    { "action": "Call", "player": "player_1", "elapsed_time": 0, "amount": 200 }
                ],
                "flop": [
                    { "action": "Check", "player": "player_1", "elapsed_time": 0 },
                    { "action": "Raise", "player": "player_2", "elapsed_time": 0, "amount": 200 },
                    { "action": "Raise", "player": "player_1", "elapsed_time": 0, "amount": 600 },
                    { "action": "Call", "player": "player_2", "elapsed_time": 0, "amount": 400 }
                ],
                "turn": [
                    { "action": "Check", "player": "player_1", "elapsed_time": 0 },
                    { "action": "Check", "player": "player_2", "elapsed_time": 0 }
                ],
                "river": [
                    { "action": "Check", "player": "player_1", "elapsed_time": 0 },
                    { "action": "Check", "player": "player_2", "elapsed_time": 0 }
                ]
            },
            "board": [
                { "shortName": "AS", "rank": "Ace", "suit": "Spade" },
                { "shortName": "AC", "rank": "Ace", "suit": "Club" },
                { "shortName": "3C", "rank": "Three", "suit": "Club" },
                { "shortName": "4C", "rank": "Four", "suit": "Club" },
                { "shortName": "8D", "rank": "Eight", "suit": "Diamond" }
            ],
            "hands": {
                "player_1": [
                    { "shortName": "AH", "rank": "Ace", "suit": "Heart" },
                    { "shortName": "KH", "rank": "King", "suit": "Heart" }
                ],
                "player_2": [
                    { "shortName": "AD", "rank": "Ace", "suit": "Diamond" },
                    { "shortName": "KC", "rank": "King", "suit": "Club" }
                ],
                "player_3": []
            },
            "pot": 1700,            
            "blinds": { "small": 50, "big": 100 },
            "won": true,
            "positions": {
                "dealer": "player_1",
                "small_blind": "player_2",
                "big_blind": "player_3"
            },
            "ranking": [["player_1", "player_2"], ["player_3"]],
            "stacks": [
                { "player": "player_1", "stack": 1050, "balance": 50 },
                { "player": "player_2", "stack": 1050, "balance": 50 },
                { "player": "player_3", "stack": 900, "balance": -100 }
            ]
        }
    )"_json;

    EXPECT_EQ(round.toJson(), expectedJson);
}

TEST(RoundTest, case3JsonRepresentationShouldBeCorrect) {
    Player player1("player_1", 1);
    Player player2("player_2", 2);
    Player player3("player_3", 3);

    player1.setStack(500);
    player2.setStack(1000);
    player3.setStack(1500);

    std::array<Player, 3> players = {player1, player2, player3};

    Round round(Blinds{50, 100}, players, {card("7C"), card("8C")}, 2);

    // Run a scenario

    // Pre-flop
    round.check(2);
    round.raise(3, 150);
    round.call(1, 100);
    round.call(2, 200);
    // Flop
    round.getBoard().setFlop({card("AS"), card("KC"), card("JC")});
    round.check(2);
    round.raise(3, 500);
    round.call(1, 300);
    round.call(2, 500);
    // Turn
    round.getBoard().setTurn(card("9C"));
    round.check(2);
    round.raise(3, 800);
    round.fold(2);
    // River
    round.getBoard().setRiver(card("8D"));
    // Showdown
    round.setPlayer3Hand({card("AD"), card("KC")});
    round.showdown();

    // language=json
    auto expectedJson = R"(
        {
            "actions": {
                "pre_flop": [
                    { "action": "Check", "player": "player_2", "elapsed_time": 0 },
                    { "action": "Raise", "player": "player_3", "elapsed_time": 0, "amount": 150 },
                    { "action": "Call", "player": "player_1", "elapsed_time": 0, "amount": 100 },
                    { "action": "Call", "player": "player_2", "elapsed_time": 0, "amount": 200 }
                ],
                "flop": [
                    { "action": "Check", "player": "player_2", "elapsed_time": 0 },
                    { "action": "Raise", "player": "player_3", "elapsed_time": 0, "amount": 500 },
                    { "action": "Call", "player": "player_1", "elapsed_time": 0, "amount": 300 },
                    { "action": "Call", "player": "player_2", "elapsed_time": 0, "amount": 500 }
                ],
                "turn": [
                    { "action": "Check", "player": "player_2", "elapsed_time": 0 },
                    { "action": "Raise", "player": "player_3", "elapsed_time": 0, "amount": 800 },
                    { "action": "Fold", "player": "player_2", "elapsed_time": 0 }
                ],
                "river": []
            },
            "board": [
                { "shortName": "AS", "rank": "Ace", "suit": "Spade" },
                { "shortName": "KC", "rank": "King", "suit": "Club" },
                { "shortName": "JC", "rank": "Jack", "suit": "Club" },
                { "shortName": "9C", "rank": "Nine", "suit": "Club" },
                { "shortName": "8D", "rank": "Eight", "suit": "Diamond" }
            ],
            "hands": {
                "player_1": [
                    { "shortName": "7C", "rank": "Seven", "suit": "Club" },
                    { "shortName": "8C", "rank": "Eight", "suit": "Club" }
                ],
                "player_2": [],
                "player_3": [                
                    { "shortName": "AD", "rank": "Ace", "suit": "Diamond" },
                    { "shortName": "KC", "rank": "King", "suit": "Club" }                    
                ]
            },
            "pot": 2700,            
            "blinds": { "small": 50, "big": 100 },
            "won": true,
            "positions": {
                "dealer": "player_2",
                "small_blind": "player_3",
                "big_blind": "player_1"
            },
            "ranking": [["player_1"], ["player_3"], ["player_2"]],
            "stacks": [
                { "player": "player_1", "stack": 1500, "balance": 1000 },
                { "player": "player_2", "stack": 300, "balance": -700 },
                { "player": "player_3", "stack": 1200, "balance": -300 }
            ]
        }
    )"_json;

    EXPECT_EQ(round.toJson(), expectedJson);
}

//  @todo add all-in scenario and showdown case

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
