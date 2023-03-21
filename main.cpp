#include <iostream>

#include <opencv4/opencv2/imgcodecs.hpp>

#include <nlohmann/json.hpp>

#include <Logger.hpp>
#include <Server.hpp>
#include <WinamaxScraper.hpp>

using Scraper::WinamaxScraper;
using Websockets::HttpRequest;
using Websockets::HttpResponse;
using Websockets::Server;

using json = nlohmann::json;

auto run(uWS::App& server) -> void {
    server
        .listen(9001,
                [](auto* listen_socket) {
                    if (listen_socket) { std::cout << "Listening on port " << 9001 << std::endl; }
                })
        .run();
}

auto main() -> int {
    WinamaxScraper scraper;

    LOG_DEBUG(Logger::Quill::getLogger(), "Main application");

    auto getWindowsNameHandler = [&](HttpResponse* response, HttpRequest* request) {
        LOG_DEBUG(Logger::Quill::getLogger(), "[{}] {}", request->getCaseSensitiveMethod(), request->getFullUrl());

        json windowsJson = {{"windows", scraper.getWindowsName()}};

        response->writeHeader("Content-Type", "application/json")->end(windowsJson.dump());
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

            response->writeHeader("Content-Type", "image/jpeg")->end(data);
        } catch (std::invalid_argument error)
        { response->writeHeader("Content-Type", "text/plain")->writeStatus("500 Error")->end(error.what()); }
    };

    std::thread wsServerThread([&]() {
        Server server;

        // Define server endpoint
        server.getInstance().get("/windows", getWindowsNameHandler).get("/screenshot", getScreenshotHandler);

        server.run();
    });

    wsServerThread.join();

    return EXIT_SUCCESS;
}
