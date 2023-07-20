#include <gtest/gtest.h>

#include <game_handler/Player.hpp>
#include <game_handler/RoundAction.hpp>

using GameHandler::Player;
using GameHandler::RoundAction;
using GameHandler::seconds;

using enum RoundAction::ActionType;

class RoundActionTest : public ::testing::Test {};

TEST(RoundActionTest, jsonRepresentationShouldBeCorrect) {
    // language=json
    auto expectedJson = R"(
        {
            "action": "Bet",
            "player_name": "player_1",
            "elapsed_time": 11,
            "amount": 1500
        }
    )"_json;

    EXPECT_EQ(RoundAction(BET, Player("player_1"), seconds(11), 1500).toJson(), expectedJson);
}
