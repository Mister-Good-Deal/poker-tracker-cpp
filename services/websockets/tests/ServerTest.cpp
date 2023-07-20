#include <chrono>

#include <gtest/gtest.h>

#include <logger/Logger.hpp>
#include <websockets/Server.hpp>

using Websockets::HttpRequest;
using Websockets::HttpResponse;
using Websockets::Server;

class ServerTest : public ::testing::Test {};

TEST(ServerTest, todo) {
    Server* serverPtr = nullptr;

    std::thread wsServerThread([&]() {
        Server server;

        serverPtr = &server;

        server.run();
    });

    wsServerThread.detach();

    std::this_thread::sleep_for(std::chrono::seconds(1));
    serverPtr->publish("test", "test data");

    std::this_thread::sleep_for(std::chrono::seconds(1));
    serverPtr->close();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    LOG_DEBUG(Logger::Quill::getLogger(), "Test end");
}
