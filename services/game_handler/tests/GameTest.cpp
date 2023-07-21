#include <gtest/gtest.h>

#include <game_handler/CardFactory.hpp>
#include <game_handler/Game.hpp>

using GameHandler::Board;
using GameHandler::Game;
using GameHandler::Player;
using GameHandler::seconds;
using GameHandler::Factory::card;

class GameTest : public ::testing::Test {};

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
TEST(GameTest, jsonRepresentationShouldBeCorrect) {
    Game game;

    game.setBuyIn(10);
    game.setMultipliers(3);
    game.init("player_1", "player_2", "player_3");

    // Shortcuts for players names
    Player& player1 = game.getPlayer1();
    Player& player2 = game.getPlayer2();
    Player& player3 = game.getPlayer3();

    // Run a scenario

    // Round 1
    auto& round1 = game.getCurrentRound();

    // Pre-flop
    round1.start();
    player1.setHand(card("AH"), card("KH"));
    round1.setHand(player1.getHand());
    round1.check(player1);
    round1.bet(player2, 100);
    round1.fold(player3);
    round1.call(player1, 100);
    round1.endStreet();
    // Flop
    round1.getBoard().setFlop({card("AS"), card("AC"), card("3C")});
    round1.check(player1);
    round1.bet(player2, 200);
    round1.bet(player1, 600);
    round1.fold(player2);
    round1.end(player1);

    // Round 2

    game.newRound();

    auto& round2 = game.getCurrentRound();

    // Pre-flop
    round2.start();
    player1.setHand(card("AH"), card("AS"));
    round2.setHand(player1.getHand());
    round2.bet(player1, 200);
    round2.bet(player2, 5000);
    round2.bet(player3, 5000);
    round2.call(player1, 4800);
    round2.endStreet();
    // Flop
    round2.getBoard().setFlop({card("AS"), card("KS"), card("KH")});
    // Turn
    round2.getBoard().setTurn(card("8C"));
    // River
    round2.getBoard().setRiver(card("7C"));
    round2.end(player1);

    // End game
    game.setWinner(player1);
    game.end();

    // language=json
    auto expectedJson = R"(
        {
            "rounds": [
                {
                    "actions": {
                        "pre_flop": [
                            { "action": "Check", "player_name": "player_1", "elapsed_time": 0 },
                            { "action": "Bet", "player_name": "player_2", "elapsed_time": 0, "amount": 100 },
                            { "action": "Fold", "player_name": "player_3", "elapsed_time": 0 },
                            { "action": "Call", "player_name": "player_1", "elapsed_time": 0, "amount": 100 }
                        ],
                        "flop": [
                            { "action": "Check", "player_name": "player_1", "elapsed_time": 0 },
                            { "action": "Bet", "player_name": "player_2", "elapsed_time": 0, "amount": 200 },
                            { "action": "Bet", "player_name": "player_1", "elapsed_time": 0, "amount": 600 },
                            { "action": "Fold", "player_name": "player_2", "elapsed_time": 0 }
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
                },
                {
                    "actions": {
                        "pre_flop": [
                            { "action": "Bet", "player_name": "player_1", "elapsed_time": 0, "amount": 200 },
                            { "action": "Bet", "player_name": "player_2", "elapsed_time": 0, "amount": 5000 },
                            { "action": "Bet", "player_name": "player_3", "elapsed_time": 0, "amount": 5000 },
                            { "action": "Call", "player_name": "player_1", "elapsed_time": 0, "amount": 4800 }
                        ],
                        "flop": [],
                        "turn": [],
                        "river": []
                    },
                    "board": [
                        { "shortName": "AS", "rank": "Ace", "suit": "Spade" },
                        { "shortName": "KS", "rank": "King", "suit": "Spade" },
                        { "shortName": "KH", "rank": "King", "suit": "Heart" },
                        { "shortName": "8C", "rank": "Eight", "suit": "Club" },
                        { "shortName": "7C", "rank": "Seven", "suit": "Club" }
                    ],
                    "hand": [
                        { "shortName": "AH", "rank": "Ace", "suit": "Heart" },
                        { "shortName": "AS", "rank": "Ace", "suit": "Spade" }
                    ],
                    "pot": 15000,
                    "bet": 5000,
                    "won": true,
                    "winner": "player_1"
                }
            ],
            "players": ["player_1", "player_2", "player_3"],
            "winner": "player_1",
            "buy_in": 10,
            "multipliers": 3,
            "won": true,
            "balance": 20,
            "duration": 0,            
            "complete": true            
        }
    )"_json;

    EXPECT_EQ(game.toJson(), expectedJson);
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
