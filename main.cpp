#include <iostream>

#include <opencv4/opencv2/imgcodecs.hpp>

#include <nlohmann/json.hpp>

#include <Logger.hpp>
#include <Server.hpp>
#include <WinamaxScraper.hpp>
#include <ranges>

using Scraper::WinamaxScraper;
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

    WinamaxScraper scraper;

    LOG_DEBUG(Logger::Quill::getLogger(), "Main application");

    auto getWindowsNameHandler = [&](HttpResponse* response, HttpRequest* request) {
        LOG_DEBUG(Logger::Quill::getLogger(), "[{}] {}", request->getCaseSensitiveMethod(), request->getFullUrl());

        auto windows = json::array();

        for_each(scraper.getWindowsName(), [&](const auto& window) {
            json windowObject = {{"title", std::get<0>(window)}, {"ID", std::get<1>(window)}};

            windows.emplace_back(windowObject);
        });

        json windowsJson = {{"windows", windows}};

        response->writeHeader("Content-Type", "application/json")
            ->writeHeader("Access-Control-Allow-Origin", "*")
            ->end(windowsJson.dump());
    };

    auto getScreenshotHandler = [&](HttpResponse* response, HttpRequest* request) {
        try
        {
            LOG_DEBUG(Logger::Quill::getLogger(), "[{}] {}", request->getCaseSensitiveMethod(), request->getFullUrl());
            LOG_DEBUG(Logger::Quill::getLogger(), "windows_name = {}", request->getQuery("windows_name"));

            auto                 image = scraper.getScreenshot(request->getQuery("windows_name"));
            std::vector<uint8_t> buffer;
            std::string          data;

            cv::imencode(".jpeg", image, buffer);

            std::copy(buffer.cbegin(), buffer.cend(), std::back_inserter(data));

            response->writeHeader("Content-Type", "image/jpeg")->writeHeader("Access-Control-Allow-Origin", "*")->end(data);
        } catch (std::invalid_argument error)
        { response->writeHeader("Content-Type", "text/plain")->writeStatus("500 Error")->end(error.what()); }
    };

    auto echoHandler = [&](WsResource ws, std::string_view message, OpCode opCode) {
        LOG_DEBUG(Logger::Quill::getLogger(), "[WS] request from {} with opCode = {}", ws->getRemoteAddress(), opCodeToString(opCode));

        json echoJson = {{"message", message}};

        ws->send(echoJson.dump(), opCode, true);
    };

    std::thread wsServerThread([&]() {
#if defined(_WIN32)
        quill::init_signal_handler();
#endif

        quill::detail::set_thread_name("ServerThread");

        Server server;

        // Define server endpoints
        server.getInstance()
            .get("/windows", getWindowsNameHandler)
            .get("/screenshot", getScreenshotHandler)
            .ws<PerSocketData>("/echo", {.message = echoHandler});

        server.run();
    });

    wsServerThread.join();

    return EXIT_SUCCESS;
}
