#include <chrono>

#include <gtest/gtest.h>

#include <Server.hpp>

using Websockets::Server;

class ServerTest : public ::testing::Test {};

TEST(ServerTest, todo) {
    Server server;

    server.run();

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        server.publish("test", "test data");
    }
}
