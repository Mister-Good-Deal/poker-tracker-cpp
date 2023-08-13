#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <opencv4/opencv2/videoio.hpp>

#include <game_session/Session.hpp>
#include <logger/Logger.hpp>
#include <utilities/Macros.hpp>

using GameSession::Game;
using GameSession::milliseconds;
using GameSession::Session;
using ::testing::AtLeast;

class SessionMock : public Session {
    public:
        SessionMock(const std::string_view& roomName, uint64_t windowId) : Session(roomName, windowId) {}

        // Mocked methods
        MOCK_METHOD(cv::Mat, _getScreenshot, (), (override));

        auto getNextFrame() -> cv::Mat {
            cv::Mat frame;

            if (!video.isOpened()) {
                if (!video.open(std::string(WINAMAX_RESOURCES_DIR) + "/game_sample_3840_1080_x8.webm")) {
                    LOG_ERROR(Logger::Quill::getLogger(), "Failed to open video");
                }
            }

            if (!video.set(cv::CAP_PROP_POS_MSEC, GameSession::TICK_RATE.count() * ++tickCount)) {
                LOG_ERROR(Logger::Quill::getLogger(), "Failed to set video position");
            }

            if (!video.read(frame)) { LOG_ERROR(Logger::Quill::getLogger(), "Cannot read video frame"); }
            if (frame.empty()) { LOG_ERROR(Logger::Quill::getLogger(), "Empty frame"); }

            DISPLAY_VIDEO("video", frame);
            
            //            DISPLAY_IMAGE("frame", frame);

            return frame;
        }

    private:
        cv::VideoCapture video;
        int32_t          tickCount = 0;
};

class SessionTest : public ::testing::Test {};

TEST(SessionTest, readVideo) {
    SessionMock session("Winamax", 0);

    session.getGame().setBuyIn(10);

    // tell Google Mock to return consecutive frames
    EXPECT_CALL(session, _getScreenshot()).Times(AtLeast(1)).WillRepeatedly([&]() { return session.getNextFrame(); });

    session.run();
}
