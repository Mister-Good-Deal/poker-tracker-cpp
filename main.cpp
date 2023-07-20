#include <iostream>

#include <opencv4/opencv2/imgcodecs.hpp>

#include <nlohmann/json.hpp>

#include <logger/Logger.hpp>
#include <ranges>
#include <scraper/Model.hpp>
#include <websockets/Server.hpp>

using Scraper::WindowInfo;
using std::ranges::for_each;
using Websockets::HttpRequest;
using Websockets::HttpResponse;
using Websockets::OpCode;
using Websockets::opCodeToString;
using Websockets::PerSocketData;
using Websockets::Server;
using Websockets::WsResource;

using json = nlohmann::json;

auto main() -> int {
#if defined(_WIN32)
    quill::init_signal_handler();
#endif

    quill::detail::set_thread_name("MainThread");

    Scraper::Model scraper("Winamax", {0, 0});
    Server*        serverPtr = nullptr;

    LOG_DEBUG(Logger::Quill::getLogger(), "Main application");

    auto getWindowsTitleHandler = [&](HttpResponse* response, HttpRequest* request) {
        LOG_DEBUG(Logger::Quill::getLogger(), "[{}] {}", request->getCaseSensitiveMethod(), request->getFullUrl());

        auto windows = json::array();

        for (const auto& [id, window] : scraper.getActiveWindows())
        {
            json windowObject = {{"title", window.title}, {"id", std::to_string(window.id)}};

            windows.emplace_back(windowObject);
        }

        json windowsJson = {{"windows", windows}};

        response->writeHeader("Content-Type", "application/json")
            ->writeHeader("Access-Control-Allow-Origin", "*")
            ->end(windowsJson.dump());
    };

    auto getScreenshotHandler = [&](HttpResponse* response, HttpRequest* request) {
        try
        {
            LOG_DEBUG(Logger::Quill::getLogger(), "[{}] {}", request->getCaseSensitiveMethod(), request->getFullUrl());
            LOG_DEBUG(Logger::Quill::getLogger(), "window_id = {}", request->getQuery("window_id"));

            if (request->getQuery("window_id").empty()) { throw std::invalid_argument("window_id param is empty"); }

            auto                 image = scraper.getScreenshot(std::stoul(request->getQuery("window_id").data()));
            std::vector<uint8_t> buffer;
            std::string          data;

            cv::imencode(".jpeg", image, buffer);

            std::copy(buffer.cbegin(), buffer.cend(), std::back_inserter(data));

            response->writeHeader("Content-Type", "image/jpeg")->writeHeader("Access-Control-Allow-Origin", "*")->end(data);
        } catch (const std::invalid_argument& error)
        {
            response->writeStatus("500 Error")
                ->writeHeader("Content-Type", "text/plain")
                ->writeHeader("Access-Control-Allow-Origin", "*")
                ->end(error.what());
        }
    };

    auto getGameInfoHandler = [&](HttpResponse* response, HttpRequest* request) {
        try
        {
            LOG_DEBUG(Logger::Quill::getLogger(), "[{}] {}", request->getCaseSensitiveMethod(), request->getFullUrl());
            LOG_DEBUG(Logger::Quill::getLogger(), "window_id = {}", request->getQuery("window_id"));

            if (request->getQuery("window_id").empty()) { throw std::invalid_argument("window_id param is empty"); }

            auto image = scraper.getScreenshot(std::stoul(request->getQuery("window_id").data()));

            json windowInfo = {{"window", {{"width", image.cols}, {"height", image.rows}}}};

            response->writeHeader("Content-Type", "application/json")
                ->writeHeader("Access-Control-Allow-Origin", "*")
                ->end(windowInfo.dump());
        } catch (const std::invalid_argument& error)
        {
            response->writeStatus("500 Error")
                ->writeHeader("Content-Type", "text/plain")
                ->writeHeader("Access-Control-Allow-Origin", "*")
                ->end(error.what());
        }
    };

    auto gameStart = [&](WsResource ws) {
        LOG_DEBUG(Logger::Quill::getLogger(), "[WS] open socket");

        ws->subscribe("test");
    };

    auto gameAction = [&](WsResource ws, std::string_view message, OpCode opCode) {
        LOG_DEBUG(Logger::Quill::getLogger(), "[WS] message with message = {}", message);

        auto jsonMessage = json::parse(message);

        ws->subscribe(fmt::format("game_{}", jsonMessage["window_id"].get<std::string>()));

        json echoJson = {{"message", jsonMessage}};

        ws->send(echoJson.dump(), opCode, true);
    };

    std::thread wsServerThread([&]() {
#if defined(_WIN32)
        quill::init_signal_handler();
#endif
        quill::detail::set_thread_name("ServerThread");

        Server server;

        serverPtr = &server;

        // Define server endpoints
        server.getInstance()
            .get("/windows", getWindowsTitleHandler)
            .get("/screenshot", getScreenshotHandler)
            .get("/game", getGameInfoHandler)
            .ws<PerSocketData>("/run", {.open = gameStart, .message = gameAction});

        server.run();
    });

    wsServerThread.detach();

    int i = 0;

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(10));

        json test = {{"message", fmt::format("test data {}", ++i)}};
        serverPtr->publish("test", test.dump());
    }

    return EXIT_SUCCESS;
}
