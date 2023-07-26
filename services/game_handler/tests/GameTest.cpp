#include <gtest/gtest.h>

#include <game_handler/CardFactory.hpp>
#include <game_handler/Game.hpp>

using GameHandler::Blinds;
using GameHandler::Board;
using GameHandler::Game;
using GameHandler::Hand;
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

    // Run a scenario

    // Round 1
    {
        auto blinds = Blinds{50, 100};
        auto hand   = Hand(card("AH"), card("KH"));

        game.newRound(hand, blinds);

        auto& round1 = game.getCurrentRound();

        // Pre-flop
        round1.check(1);
        round1.bet(2, 100);
        round1.fold(3);
        round1.call(1, 100);
        // Flop
        round1.getBoard().setFlop({card("AS"), card("AC"), card("3C")});
        round1.check(1);
        round1.bet(2, 200);
        round1.bet(1, 600);
        round1.fold(2);
    }

    // Round 2
    {
        auto blinds = Blinds{100, 200};
        auto hand   = Hand(card("AH"), card("AS"));

        game.newRound(hand, blinds);

        auto& round2 = game.getCurrentRound();

        // Pre-flop
        round2.bet(1, 200);
        round2.bet(2, 5000);
        round2.bet(3, 5000);
        round2.call(1, 4800);
        // Flop
        round2.getBoard().setFlop({card("AS"), card("KS"), card("KH")});
        // Turn
        round2.getBoard().setTurn(card("8C"));
        // River
        round2.getBoard().setRiver(card("7C"));
    }

    // End game @todo automate this
    game.setWinner(game.getPlayer1());
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
                    "blinds": { "small": 50, "big": 100 },
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
                    "blinds": { "small": 100, "big": 200 },
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
