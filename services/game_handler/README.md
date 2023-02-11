# Game handler

This service represents the game status and historic.

It logs all the game's events and can serialize them in JSON format.

*Libraries used*

- [nlohmann-json](https://github.com/nlohmann/json)
- [GoogleTest](https://github.com/google/googletest)

## Architecture

The Game Handler service splits all Poker's game attributes (Board, Player, Hand, etc ...) into OOP classes.

The classes header definition are located in `game_handler/include` with **.hpp** file extension.

The classes implementation are located in `game_handler/src` with **.cpp** file extension.

The classes tests are located in `game_handler/tests` with **.cpp** file extension using GoogleTest framework.

The whole service is built into a static library

## Classes

- **Card**: Represent a card (Ace of Spade, Seven of Heart, etc ...)
- **CardFactory** [*using **Card***]: Factory to build a cart with its short name (AS, 7H, etc ...)
- **Hand** [*using **Card***]: Represent a player's hand (2 cards)
- **Player** [*using **Hand***]: Represent a player with his name and stack
- **Board** [*using **Hand***]: Represent the game board at different streets
- **RoundAction** [*using **Player***]: Represent a player action in the game (Bet, Check, Call, Fold)
- **Round** [*using **RoundAction** and **Board***]: Represent a game round with all players actions during it
- **Game** [*using **Round***]: Represent the whole game until a player win with all the game's rounds.