#pragma once

#include <uwebsockets/App.h>

namespace Websockets {
    using uWS::OpCode;

    struct PerSocketData {
        public:
            int something;
    };

    class Server {
        public:
            Server();

            auto run() -> void;
            auto publish(std::string_view topic, std::string_view data, OpCode code, bool compress = false) -> bool;

        private:
            uWS::App _server;
    };
}  // namespace Websockets