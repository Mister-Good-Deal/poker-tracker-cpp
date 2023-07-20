#include <gtest/gtest.h>

#include <game_session/Session.hpp>

using GameSession::Game;
using GameSession::Session;

class SessionMock : public Session {
    public:
        SessionMock(const std::string_view& roomName, uint64_t windowId) : Session(roomName, windowId) {}

        // Proxy protected methods
        auto harvestGameInfo(const cv::Mat& screenshot) -> void { _harvestGameInfo(screenshot); }
};

class SessionTest : public ::testing::Test {};

TEST(SessionTest, winamaxHarvestInfo) {
    SessionMock session("Winamax", 0);

    session.getGame().setBuyIn(10);

    cv::Mat screenshot = cv::imread(std::string(WINAMAX_IMAGES_DIR) + "/screen_4.png");

    session.harvestGameInfo(screenshot);

    EXPECT_STREQ(session.getGame().getPlayer1().getName().c_str(), "_Mister_");
    EXPECT_STREQ(session.getGame().getPlayer2().getName().c_str(), "sucre_461406");
}
