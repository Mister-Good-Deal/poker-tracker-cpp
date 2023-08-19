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

    Player player1("player 1", 1);
    Player player2("player 2", 2);
    Player player3("player 3", 3);

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
    round.raise(2, 200);
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
                    { "action": "Raise", "player": "player_2", "elapsed_time": 0, "amount": 200 },
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
    Player player1("player 1", 1);
    Player player2("player 2", 2);
    Player player3("player 3", 3);

    player1.setStack(1000);
    player2.setStack(1000);
    player3.setStack(1000);

    std::array<Player, 3> players = {player1, player2, player3};

    Round round(Blinds{50, 100}, players, {card("AH"), card("KH")}, 1);

    // Run a scenario

    // Pre-flop
    round.check(1);
    round.raise(2, 200);
    round.fold(3);
    round.call(1, 200);
    // Flop
    round.getBoard().setFlop({card("AS"), card("AC"), card("3C")});
    round.check(1);
    round.raise(2, 200);
    round.raise(1, 600);
    round.call(2, 600);
    // Turn
    round.getBoard().setTurn(card("4C"));
    round.check(1);
    round.check(2);
    // River
    round.getBoard().setRiver(card("8D"));
    round.check(1);
    round.check(2);
    // Showdown
    EXPECT_TRUE(round.waitingShowdown());

    round.setPlayerHand({card("AD"), card("KC")}, 2);
    round.showdown();

    // language=json
    auto expectedJson = R"(
        {
            "actions": {
                "pre_flop": [
                    { "action": "Check", "player": "player_1", "elapsed_time": 0 },
                    { "action": "Raise", "player": "player_2", "elapsed_time": 0, "amount": 200 },
                    { "action": "Fold", "player": "player_3", "elapsed_time": 0 },
                    { "action": "Call", "player": "player_1", "elapsed_time": 0, "amount": 200 }
                ],
                "flop": [
                    { "action": "Check", "player": "player_1", "elapsed_time": 0 },
                    { "action": "Raise", "player": "player_2", "elapsed_time": 0, "amount": 200 },
                    { "action": "Raise", "player": "player_1", "elapsed_time": 0, "amount": 600 },
                    { "action": "Call", "player": "player_2", "elapsed_time": 0, "amount": 600 }
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
    Player player1("player 1", 1);
    Player player2("player 2", 2);
    Player player3("player 3", 3);

    player1.setStack(500);
    player2.setStack(1000);
    player3.setStack(1500);

    std::array<Player, 3> players = {player1, player2, player3};

    Round round(Blinds{50, 100}, players, {card("7C"), card("8C")}, 2);

    // Run a scenario

    // Pre-flop
    round.check(2);
    round.raise(3, 200);
    round.call(1, 200);
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
    EXPECT_TRUE(round.waitingShowdown());

    round.setPlayerHand({card("AD"), card("KC")}, 3);
    round.showdown();

    // language=json
    auto expectedJson = R"(
        {
            "actions": {
                "pre_flop": [
                    { "action": "Check", "player": "player_2", "elapsed_time": 0 },
                    { "action": "Raise", "player": "player_3", "elapsed_time": 0, "amount": 200 },
                    { "action": "Call", "player": "player_1", "elapsed_time": 0, "amount": 200 },
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

TEST(RoundTest, case4JsonRepresentationShouldBeCorrect) {
    Player player1("player 1", 1);
    Player player2("player 2", 2);
    Player player3("player 3", 3);

    player1.setStack(300);
    player2.setStack(300);
    player3.setStack(300);

    std::array<Player, 3> players = {player1, player2, player3};

    Round round(Blinds{10, 20}, players, {card("TH"), card("9C")}, 2);

    // Run a scenario

    // Pre-flop
    round.fold(2);
    round.raise(3, 300);
    round.fold(1);

    // language=json
    auto expectedJson = R"(
        {
            "actions": {
                "pre_flop": [
                    { "action": "Fold", "player": "player_2", "elapsed_time": 0 },
                    { "action": "Raise", "player": "player_3", "elapsed_time": 0, "amount": 300 },
                    { "action": "Fold", "player": "player_1", "elapsed_time": 0 }
                ],
                "flop": [],
                "turn": [],
                "river": []
            },
            "board": [],
            "hands": {
                "player_1": [
                    { "shortName": "TH", "rank": "Ten", "suit": "Heart" },
                    { "shortName": "9C", "rank": "Nine", "suit": "Club" }
                ],
                "player_2": [],
                "player_3": []
            },
            "pot": 320,            
            "blinds": { "small": 10, "big": 20 },
            "won": false,
            "positions": {
                "dealer": "player_2",
                "small_blind": "player_3",
                "big_blind": "player_1"
            },
            "ranking": [["player_3"], ["player_1"], ["player_2"]],
            "stacks": [
                { "player": "player_1", "stack": 280, "balance": -20 },
                { "player": "player_2", "stack": 300, "balance": 0 },
                { "player": "player_3", "stack": 320, "balance": 20 }
            ]
        }
    )"_json;

    EXPECT_EQ(round.toJson(), expectedJson);
}

TEST(RoundTest, case5JsonRepresentationShouldBeCorrect) {
    Player player1("player 1", 1);
    Player player2("player 2", 2);
    Player player3("player 3", 3);
    
    player1.setStack(280);
    player2.setStack(300);
    player3.setStack(320);

    std::array<Player, 3> players = {player1, player2, player3};

    Round round(Blinds{10, 20}, players, {card("2H"), card("3C")}, 3);

    // Run a scenario

    // Pre-flop
    round.call(3, 20);
    round.call(1, 20);
    round.raise(2, 300);
    round.fold(3);
    EXPECT_FALSE(round._isStreetOver());
    EXPECT_FALSE(round.waitingShowdown());
    round.fold(1);

    // language=json
    auto expectedJson = R"(
        {
            "actions": {
                "pre_flop": [
                    { "action": "Call", "player": "player_3", "elapsed_time": 0, "amount": 20 },
                    { "action": "Call", "player": "player_1", "elapsed_time": 0, "amount": 20 },
                    { "action": "Raise", "player": "player_2", "elapsed_time": 0, "amount": 300 },
                    { "action": "Fold", "player": "player_3", "elapsed_time": 0 },
                    { "action": "Fold", "player": "player_1", "elapsed_time": 0 }
                ],
                "flop": [],
                "turn": [],
                "river": []
            },
            "board": [],
            "hands": {
                "player_1": [
                    { "shortName": "2H", "rank": "Two", "suit": "Heart" },
                    { "shortName": "3C", "rank": "Three", "suit": "Club" }
                ],
                "player_2": [],
                "player_3": []
            },
            "pot": 340,            
            "blinds": { "small": 10, "big": 20 },
            "won": false,
            "positions": {
                "dealer": "player_3",
                "small_blind": "player_1",
                "big_blind": "player_2"
            },
            "ranking": [["player_2"], ["player_1"], ["player_3"]],
            "stacks": [
                { "player": "player_1", "stack": 260, "balance": -20 },
                { "player": "player_2", "stack": 340, "balance": 40 },
                { "player": "player_3", "stack": 300, "balance": -20 }
            ]
        }
    )"_json;

    EXPECT_EQ(round.toJson(), expectedJson);
}

TEST(RoundTest, GeneratedByAI_case1JsonRepresentationShouldBeCorrect) {
    Player player1("player 1", 1);
    Player player2("player 2", 2);
    Player player3("player 3", 3);

    player1.setStack(1000);
    player2.setStack(1000);
    player3.setStack(1000);

    std::array<Player, 3> players = {player1, player2, player3};

    Round round(Blinds{50, 100}, players, {card("QC"), card("5H")}, 1);

    // Run a scenario

    // Pre-flop
    round.raise(1, 300);
    round.fold(2);
    round.call(3, 300);
    // End of pre-flop: Pot: 650,  Street pot: 650
    // Stacks:      player_1=700, player_2=950, player_3=700
    // Total bet:   player_1=300,  player_2=50, player_3=300

    // Flop
    round.getBoard().setFlop({card("8D"), card("2S"), card("KC")});
    round.check(1);
    round.check(3);
    // End of flop: Pot: 650,  Street pot: 0
    // Stacks:      player_1=700, player_2=950, player_3=700
    // Total bet:   player_1=300,  player_2=50, player_3=300

    // Turn
    round.getBoard().setTurn(card("6H"));
    round.check(1);
    round.raise(3, 400);
    round.call(1, 400);
    // End of turn: Pot: 1450,  Street pot: 800
    // Stacks:      player_1=300, player_2=950, player_3=300
    // Total bet:   player_1=700,  player_2=50, player_3=700

    // River
    round.getBoard().setRiver(card("9S"));
    round.check(1);
    round.check(3);
    // End of river: Pot: 1450,  Street pot: 0
    // Stacks:       player_1=300, player_2=950, player_3=300
    // Total bet:    player_1=700,  player_2=50, player_3=700

    // Showdown
    EXPECT_TRUE(round.waitingShowdown());

    round.setPlayerHand({card("7C"), card("4S")}, 3);  // Player 3's hand
    round.showdown();
    // Player 1 wins the pot.
    // Stacks after showdown: player_1=1750, player_2=950, player_3=300

    // language=json
    auto expectedJson = R"(
        {
            "actions": {
                "pre_flop": [
                    { "action": "Raise", "player": "player_1", "elapsed_time": 0, "amount": 300 },
                    { "action": "Fold", "player": "player_2", "elapsed_time": 0 },
                    { "action": "Call", "player": "player_3", "elapsed_time": 0, "amount": 300 }
                ],
                "flop": [
                    { "action": "Check", "player": "player_1", "elapsed_time": 0 },
                    { "action": "Check", "player": "player_3", "elapsed_time": 0 }
                ],
                "turn": [
                    { "action": "Check", "player": "player_1", "elapsed_time": 0 },
                    { "action": "Raise", "player": "player_3", "elapsed_time": 0, "amount": 400 },
                    { "action": "Call", "player": "player_1", "elapsed_time": 0, "amount": 400 }
                ],
                "river": [
                    { "action": "Check", "player": "player_1", "elapsed_time": 0 },
                    { "action": "Check", "player": "player_3", "elapsed_time": 0 }
                ]
            },
            "board": [
                { "shortName": "8D", "rank": "Eight", "suit": "Diamond" },
                { "shortName": "2S", "rank": "Two", "suit": "Spade" },
                { "shortName": "KC", "rank": "King", "suit": "Club" },
                { "shortName": "6H", "rank": "Six", "suit": "Heart" },
                { "shortName": "9S", "rank": "Nine", "suit": "Spade" }
            ],
            "hands": {
                "player_1": [
                    { "shortName": "QC", "rank": "Queen", "suit": "Club" },
                    { "shortName": "5H", "rank": "Five", "suit": "Heart" }
                ],
                "player_2": [],
                "player_3": [
                    { "shortName": "7C", "rank": "Seven", "suit": "Club" },
                    { "shortName": "4S", "rank": "Four", "suit": "Spade" }
                ]
            },
            "pot": 1450,
            "blinds": { "small": 50, "big": 100 },
            "won": true,
            "positions": {
                "dealer": "player_1",
                "small_blind": "player_2",
                "big_blind": "player_3"
            },
            "ranking": [["player_1"], ["player_3"], ["player_2"]],
            "stacks": [
                { "player": "player_1", "stack": 1750, "balance": 750 },
                { "player": "player_2", "stack": 950, "balance": -50 },
                { "player": "player_3", "stack": 300, "balance": -700 }
            ]
        }
    )"_json;

    EXPECT_EQ(round.toJson(), expectedJson);
}

TEST(RoundTest, GeneratedByAI_case6JsonRepresentationShouldBeCorrect) {
    Player player1("player 1", 1);
    Player player2("player 2", 2);
    Player player3("player 3", 3);

    player1.setStack(1000);
    player2.setStack(1000);
    player3.setStack(1000);

    std::array<Player, 3> players = {player1, player2, player3};

    Round round(Blinds{50, 100}, players, {card("AD"), card("6H")}, 2);

    // Run a scenario

    // Pre-flop
    round.raise(2, 300);
    round.fold(3);
    round.call(1, 300);
    // End of Pre-flop, Street pot: 650
    EXPECT_EQ(round.getPot(), 650);
    EXPECT_EQ(round.getCurrentPlayerStack(1), 700);
    EXPECT_EQ(round.getCurrentPlayerStack(2), 700);
    EXPECT_EQ(round.getCurrentPlayerStack(3), 950);

    // Flop
    round.getBoard().setFlop({card("7D"), card("5H"), card("2S")});
    round.check(1);
    round.bet(2, 400);
    round.fold(1);
    // End of Flop, Street pot: 400

    // Player 2 wins the pot
    EXPECT_EQ(round.getPot(), 1050);
    EXPECT_EQ(players[0].getStack(), 700);
    EXPECT_EQ(players[1].getStack(), 1350);
    EXPECT_EQ(players[2].getStack(), 950);

    // language=json
    auto expectedJson = R"(
        {
            "actions": {
                "pre_flop": [
                    { "action": "Raise", "player": "player_2", "elapsed_time": 0, "amount": 300 },
                    { "action": "Fold", "player": "player_3", "elapsed_time": 0 },
                    { "action": "Call", "player": "player_1", "elapsed_time": 0, "amount": 300 }
                ],
                "flop": [
                    { "action": "Check", "player": "player_1", "elapsed_time": 0 },
                    { "action": "Bet", "player": "player_2", "elapsed_time": 0, "amount": 400 },
                    { "action": "Fold", "player": "player_1", "elapsed_time": 0 }
                ],                
                "turn": [],
                "river": []
            },
            "board": [
                { "shortName": "7D", "rank": "Seven", "suit": "Diamond" },
                { "shortName": "5H", "rank": "Five", "suit": "Heart" },
                { "shortName": "2S", "rank": "Two", "suit": "Spade" }
            ],
            "hands": {
                "player_1": [
                    { "shortName": "AD", "rank": "Ace", "suit": "Diamond" },
                    { "shortName": "6H", "rank": "Six", "suit": "Heart" }
                ],
                "player_2": [],
                "player_3": []
            },
            "pot": 1050,
            "blinds": { "small": 50, "big": 100 },
            "won": false,
            "positions": {
                "dealer": "player_2",
                "small_blind": "player_3",
                "big_blind": "player_1"
            },
            "ranking": [["player_2"], ["player_1"], ["player_3"]],
            "stacks": [
                { "player": "player_1", "stack": 700, "balance": -300 },
                { "player": "player_2", "stack": 1350, "balance": 350 },
                { "player": "player_3", "stack": 950, "balance": -50 }
            ]
        }
    )"_json;

    EXPECT_EQ(round.toJson(), expectedJson);
}

TEST(RoundTest, GeneratedByAI_case11JsonRepresentationShouldBeCorrect) {
    Player player1("player_1", 1);
    Player player2("player_2", 2);
    Player player3("player_3", 3);
    
    player1.setStack(1000);
    player2.setStack(1000);
    player3.setStack(1000);

    std::array<Player, 3> players = {player1, player2, player3};

    Round round(Blinds{50, 100}, players, {card("7D"), card("3C")}, 2);

    // Run a scenario

    // Pre-flop
    round.raise(2, 300); // The button always playing 1st
    round.fold(3);
    round.call(1, 300);
    // End of Pre-flop: Pot: 650,  Street pot: 650
    EXPECT_EQ(round.getPot(), 650);
    EXPECT_EQ(round.getCurrentPlayerStack(1), 700);
    EXPECT_EQ(round.getCurrentPlayerStack(2), 700);
    EXPECT_EQ(round.getCurrentPlayerStack(3), 950);

    // Flop
    round.getBoard().setFlop({card("4H"), card("8S"), card("JS")});
    round.bet(2, 200); // The button always playing 1st
    round.fold(1);
    // End of Flop: Pot: 850,  Street pot: 200

    // Player 2 wins the pot.
    EXPECT_EQ(round.getPot(), 850);
    EXPECT_EQ(players[0].getStack(), 700);
    EXPECT_EQ(players[1].getStack(), 1350);
    EXPECT_EQ(players[2].getStack(), 950);

    // language=json
    auto expectedJson = R"(
        {
            "actions": {
                "pre_flop": [
                    { "action": "Raise", "player": "player_2", "elapsed_time": 0, "amount": 300 },
                    { "action": "Fold", "player": "player_3", "elapsed_time": 0 },
                    { "action": "Call", "player": "player_1", "elapsed_time": 0, "amount": 300 }
                ],
                "flop": [
                    { "action": "Bet", "player": "player_2", "elapsed_time": 0, "amount": 200 },
                    { "action": "Fold", "player": "player_1", "elapsed_time": 0 }
                ],
                "turn": [],
                "river": []
            },
            "board": [
                { "shortName": "4H", "rank": "Four", "suit": "Heart" },
                { "shortName": "8S", "rank": "Eight", "suit": "Spade" },
                { "shortName": "JS", "rank": "Jack", "suit": "Spade" }
            ],
            "hands": {
                "player_1": [
                    { "shortName": "7D", "rank": "Seven", "suit": "Diamond" },
                    { "shortName": "3C", "rank": "Three", "suit": "Club" }
                ],
                "player_2": [],
                "player_3": []
            },
            "pot": 850,
            "blinds": { "small": 50, "big": 100 },
            "won": false,
            "positions": {
                "dealer": "player_2",
                "small_blind": "player_3",
                "big_blind": "player_1"
            },
            "ranking": [["player_2"], ["player_1"], ["player_3"]],
            "stacks": [
                { "player": "player_1", "stack": 700, "balance": -300 },
                { "player": "player_2", "stack": 1350, "balance": 350 },
                { "player": "player_3", "stack": 950, "balance": -50 }
            ]
        }
    )"_json;

    EXPECT_EQ(round.toJson(), expectedJson);
}


//  @todo add all-in scenario and showdown case

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
