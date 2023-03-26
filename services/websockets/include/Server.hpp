#pragma once

#include <functional>

#include <uwebsockets/App.h>

namespace Websockets {
    using uWS::HttpRequest;
    using uWS::OpCode;

    static constexpr auto opCodeToString(OpCode code) -> std::string_view {
        switch (code)
        {
            case OpCode::BINARY: return "BINARY";
            case OpCode::CLOSE: return "CLOSE";
            case OpCode::CONTINUATION: return "CONTINUATION";
            case OpCode::PING: return "PING";
            case OpCode::PONG: return "PONG";
            case OpCode::TEXT: return "TEXT";
            default: throw std::invalid_argument("The given opCode is invalid");
        }
    }

    struct PerSocketData {
        public:
            int something;
    };

    // Force SSL = false on HttpResponse type
    using HttpResponse = uWS::HttpResponse<false>;
    using WsResource   = uWS::WebSocket<false, true, PerSocketData>*;
    // Proxy function signature to pass data (std::string_view) to return to the given endpoint
    using ProxyDataFn = std::function<void(HttpResponse*, HttpRequest*)>;

    constexpr int32_t SERVER_LISTENING_PORT = 9001;
    constexpr int32_t SERVER_IDLE_TIMEOUT   = 60 * 10;

    class Server {
        public:
            Server();

            auto run() -> void;
            auto close() -> void;
            auto publish(std::string_view topic, std::string_view data, OpCode code = OpCode::TEXT, bool compress = false) -> bool;
            auto getInstance() -> uWS::App& { return _server; }

        private:
            uWS::App _server;

            auto configureWebsockets() -> void;
    };
}  // namespace Websockets