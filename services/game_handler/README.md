# Game handler

This service represents the game status and historic.

It logs all the game's events and can serialize them in JSON format.

*Libraries used*

- [nlohmann-json](https://github.com/nlohmann/json)
- [GoogleTest](https://github.com/google/googletest)

## Architecture

The Game Handler service splits all Poker's game attributes (Board, Player, Hand, etc ...) into POO classes.

The classes header definition are located in `game_handler/include` with **.hpp** file extension.

The classes implementation are located in `game_handler/src` with **.cpp** file extension.

The classes tests are located in `game_handler/tests` with **.cpp** file extension using GoogleTest framework.

The whole service is built into a static library.

## Classes

- **Card**: Represent a card (Ace of Spade, Seven of Heart, etc ...)
- **CardFactory** [*using **Card***]: Factory to build a card with its short name (`AS`, `7H`, etc ...)
- **Hand** [*using **Card***]: Represent a player's hand (2 cards)
- **Player** [*using **Hand***]: Represent a player with his name and stack
- **Board** [*using **Hand***]: Represent the game board at different streets
- **RoundAction** [*using **Player***]: Represent a player action in the game (Bet, Check, Call, Fold)
- **Round** [*using **RoundAction** and **Board***]: Represent a game round with all players actions during it
- **Game** [*using **Round***]: Represent the whole game until a player win with all the game's rounds.

## Logic

The Game Handler service is used to log all the game's events and to serialize them in JSON format.

By default, `player_1` is the hero and `player_2` and `player_3` are the villains.

The service knows the game's rules and can determine the winner of a round and process the pot distribution based only on the players
actions.

## Serialization

The Game Handler service can serialize a game into JSON format.

For the following C++ code, the JSON format is defined as follow:

**C++ code**
```cpp
#include <game_handler/CardFactory.hpp>
#include <game_handler/Game.hpp>

using GameHandler::Blinds;
using GameHandler::Board;
using GameHandler::Game;
using GameHandler::Hand;
using GameHandler::Player;
using GameHandler::seconds;
using GameHandler::Factory::card;

auto main() -> void {
    Game game;

    game.setBuyIn(10);
    game.setMultipliers(3);
    game.setInitialStack(1000);
    game.init("player_1", "player_2", "player_3");

    // Run a scenario

    // Round 1
    {
        auto& round1 = game.newRound({50, 100}, {card("AH"), card("KH")}, 1);

        // Pre-flop
        round1.check(1);
        round1.bet(2, 250);
        round1.fold(3);
        round1.call(1, 300);
        // Flop
        round1.getBoard().setFlop({card("AS"), card("AC"), card("3C")});
        round1.check(1);
        round1.bet(2, 200);
        round1.bet(1, 700);
        round1.fold(2);
        // Stacks
        // Player 1: 1000 + 1600 - 1000 = 1600
        // Player 2: 1000 - 500 = 500
        // Player 3: 1000 - 100 = 900
    }

    // Round 2
    {
        // Dealer is player 2
        auto& round2 = game.newRound({100, 200}, {card("AH"), card("AS")}, 2);

        // Pre-flop
        round2.bet(2, 500);
        round2.call(3, 500);
        round2.bet(1, 1400);
        round2.call(3, 300);
        // Flop
        round2.getBoard().setFlop({card("AS"), card("KS"), card("KH")});
        // Turn
        round2.getBoard().setTurn(card("8C"));
        // River
        round2.getBoard().setRiver(card("7C"));

        round2.setPlayer2Hand({card("TS"), card("TH")});
        round2.setPlayer3Hand({card("9S"), card("9H")});
        round2.showdown();
    }

    // End game
    game.end();
}
```

**JSON format**
```json
{
  "rounds": [
    {
      "actions": {
        "pre_flop": [
          { "action": "Check", "player_name": "player_1", "elapsed_time": 0 },
          { "action": "Bet", "player_name": "player_2", "elapsed_time": 0, "amount": 250 },
          { "action": "Fold", "player_name": "player_3", "elapsed_time": 0 },
          { "action": "Call", "player_name": "player_1", "elapsed_time": 0, "amount": 300 }
        ],
        "flop": [
          { "action": "Check", "player_name": "player_1", "elapsed_time": 0 },
          { "action": "Bet", "player_name": "player_2", "elapsed_time": 0, "amount": 200 },
          { "action": "Bet", "player_name": "player_1", "elapsed_time": 0, "amount": 700 },
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
      "hands": {
        "player_1": [
          { "shortName": "AH", "rank": "Ace", "suit": "Heart" },
          { "shortName": "KH", "rank": "King", "suit": "Heart" }
        ],
        "player_2": [],
        "player_3": []
      },
      "pot": 1600,
      "blinds": { "small": 50, "big": 100 },
      "won": true,
      "positions": {
        "dealer": "player_1",
        "small_blind": "player_2",
        "big_blind": "player_3"
      },
      "ranking": [["player_1"], ["player_2"], ["player_3"]],
      "stacks": [
        { "player": "player_1", "stack": 1600, "balance": 600 },
        { "player": "player_2", "stack": 500, "balance": -500 },
        { "player": "player_3", "stack": 900, "balance": -100 }
      ]
    },
    {
      "actions": {
        "pre_flop": [
          { "action": "Bet", "player_name": "player_2", "elapsed_time": 0, "amount": 500 },
          { "action": "Call", "player_name": "player_3", "elapsed_time": 0, "amount": 500 },
          { "action": "Bet", "player_name": "player_1", "elapsed_time": 0, "amount": 1400 },
          { "action": "Call", "player_name": "player_3", "elapsed_time": 0, "amount": 300 }
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
      "hands": {
        "player_1": [
          { "shortName": "AH", "rank": "Ace", "suit": "Heart" },
          { "shortName": "AS", "rank": "Ace", "suit": "Spade" }
        ],
        "player_2": [
          { "shortName": "TS", "rank": "Ten", "suit": "Spade" },
          { "shortName": "TH", "rank": "Ten", "suit": "Heart" }
        ],
        "player_3": [
          { "shortName": "9S", "rank": "Nine", "suit": "Spade" },
          { "shortName": "9H", "rank": "Nine", "suit": "Heart" }
        ]
      },
      "pot": 3000,
      "blinds": { "small": 100, "big": 200 },
      "won": true,
      "positions": {
        "dealer": "player_2",
        "small_blind": "player_3",
        "big_blind": "player_1"
      },
      "ranking": [["player_1"], ["player_2"], ["player_3"]],
      "stacks": [
        { "player": "player_1", "stack": 3000, "balance": 1400 },
        { "player": "player_2", "stack": 0, "balance": -500 },
        { "player": "player_3", "stack": 0, "balance": -900 }
      ]
    }
  ],
  "players": ["player_1", "player_2", "player_3"],
  "buy_in": 10,
  "multipliers": 3,
  "won": true,
  "balance": 20,
  "duration": 0,
  "complete": true
}
```