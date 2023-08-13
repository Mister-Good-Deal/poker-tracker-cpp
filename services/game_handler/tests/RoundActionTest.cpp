#include <gtest/gtest.h>

#include <game_handler/Player.hpp>
#include <game_handler/RoundAction.hpp>

using GameHandler::Player;
using GameHandler::RoundAction;
using GameHandler::seconds;

using enum RoundAction::ActionType;

class RoundActionTest : public ::testing::Test {};

TEST(RoundActionTest, jsonRepresentationForBetShouldBeCorrect) {
    // language=json
    auto expectedJson = R"(
        {
            "action": "Bet",
            "player_name": "player_1",
            "elapsed_time": 11,
            "amount": 1500
        }
    )"_json;

    EXPECT_EQ(RoundAction(BET, Player("player_1", 1), seconds(11), 1500).toJson(), expectedJson);
}

TEST(RoundActionTest, jsonRepresentationForCallShouldBeCorrect) {
    // language=json
    auto expectedJson = R"(
        {
            "action": "Call",
            "player_name": "player_3",
            "elapsed_time": 8,
            "amount": 230
        }
    )"_json;

    EXPECT_EQ(RoundAction(CALL, Player("player_3", 3), seconds(8), 230).toJson(), expectedJson);
}

TEST(RoundActionTest, jsonRepresentationForCheckShouldBeCorrect) {
    // language=json
    auto expectedJson = R"(
        {
            "action": "Check",
            "player_name": "player_1",
            "elapsed_time": 5
        }
    )"_json;

    EXPECT_EQ(RoundAction(CHECK, Player("player_1", 1), seconds(5)).toJson(), expectedJson);
}

TEST(RoundActionTest, jsonRepresentationForFoldShouldBeCorrect) {
    // language=json
    auto expectedJson = R"(
        {
            "action": "Fold",
            "player_name": "player_2",
            "elapsed_time": 3
        }
    )"_json;

    EXPECT_EQ(RoundAction(FOLD, Player("player_2", 2), seconds(3)).toJson(), expectedJson);
}

TEST(RoundActionTest, jsonRepresentationForPayBigBlindShouldBeCorrect) {
    // language=json
    auto expectedJson = R"(
        {
            "action": "Pay big blind",
            "player_name": "player_1",
            "elapsed_time": 2,
            "amount": 100
        }
    )"_json;

    EXPECT_EQ(RoundAction(PAY_BIG_BLIND, Player("player_1", 1), seconds(2), 100).toJson(), expectedJson);
}

TEST(RoundActionTest, jsonRepresentationForPaySmallBlindShouldBeCorrect) {
    // language=json
    auto expectedJson = R"(
        {
            "action": "Pay small blind",
            "player_name": "player_2",
            "elapsed_time": 1,
            "amount": 50
        }
    )"_json;

    EXPECT_EQ(RoundAction(PAY_SMALL_BLIND, Player("player_2", 2), seconds(1), 50).toJson(), expectedJson);
}

TEST(RoundActionTest, jsonRepresentationForRaiseShouldBeCorrect) {
    // language=json
    auto expectedJson = R"(
        {
            "action": "Raise",
            "player_name": "player_1",
            "elapsed_time": 4,
            "amount": 1000
        }
    )"_json;

    EXPECT_EQ(RoundAction(RAISE, Player("player_1", 1), seconds(4), 1000).toJson(), expectedJson);
}
