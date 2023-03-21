#pragma once

#include <functional>

#include <uwebsockets/App.h>

namespace Websockets {
    using uWS::HttpRequest;
    using uWS::OpCode;

    // Force SSL = false on HttpResponse type
    using HttpResponse = uWS::HttpResponse<false>;
    // Proxy function signature to pass data (std::string_view) to return to the given endpoint
    using ProxyDataFn = std::function<void(HttpResponse*, HttpRequest*)>;

    constexpr int32_t SERVER_LISTENING_PORT = 9001;
    constexpr int32_t SERVER_IDLE_TIMEOUT   = 60 * 10;

    struct PerSocketData {
        public:
            int something;
    };

    class Server {
        public:
            Server();

            auto run() -> void;
            auto close() -> void;
            auto publish(std::string_view topic, std::string_view data, OpCode code = OpCode::TEXT, bool compress = false) -> bool;

            // Endpoints proxy functions
            auto addGetEndpoint(std::string_view endpoint, const ProxyDataFn& proxy) -> void;

        private:
            uWS::App _server;

            auto configureWebsockets() -> void;
    };
}  // namespace Websockets