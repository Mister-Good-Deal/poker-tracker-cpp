#pragma once

#include <uwebsockets/App.h>

namespace Websockets {
    using uWS::OpCode;

    constexpr int32_t SERVER_LISTENING_PORT = 9001;

    struct PerSocketData {
        public:
            int something;
    };

    class Server {
        public:
            Server();

            auto run() -> void;
            auto publish(std::string_view topic, std::string_view data, OpCode code = OpCode::TEXT, bool compress = false) -> bool;

        private:
            uWS::App _server;
    };
}  // namespace Websockets