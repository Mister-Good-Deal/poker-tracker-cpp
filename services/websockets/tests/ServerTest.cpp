#include <chrono>

#include <gtest/gtest.h>

#include <Server.hpp>

using Websockets::Server;

class ServerTest : public ::testing::Test {};

TEST(ServerTest, todo) {
    Server server;

    std::thread thread(&Server::run, &server);
    thread.join();

    std::this_thread::sleep_for(std::chrono::seconds(1));
    server.publish("test", "test data");

    std::this_thread::sleep_for(std::chrono::seconds(3));
    server.close();
}
