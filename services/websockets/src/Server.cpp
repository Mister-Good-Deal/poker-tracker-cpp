#include "Server.hpp"

namespace Websockets {
    using uWS::WebSocket;

    using wsResource = WebSocket<false, true, PerSocketData>*;

    // Configure the server at constructor level to get the resource in class param (_server);
    Server::Server() :
        _server(uWS::App()
                    .get("/info",
                         [](auto* res, auto* req) {
                             res->writeHeader("Content-Type", "text/html; charset=utf-8")->end("Websocket server info");
                         })
                    .ws<PerSocketData>("/*",
                                       {.upgrade = [](auto* res, auto* req, auto* context) {},
                                        .open    = [](wsResource ws) { ws->subscribe("oh_interesting_subject"); },
                                        .message =
                                            [](wsResource ws, std::string_view message, OpCode opCode) {
                                                // @todo Parse the message and send the appropriate response
                                                ws->send(message, opCode);
                                            }

                                       })
                    .listen(9001, [](auto* listenSocket) {
                        if (listenSocket)
                        {
                            std::cout << "Listening on port " << 9001 << std::endl;
                        } else {
                            std::cout << "Failed to bind to port" << std::endl;
                        }
                    })) {}

    auto Server::publish(std::string_view topic, std::string_view data, OpCode code, bool compress) -> bool {
        return _server.publish(topic, data, code, compress);
    }

    auto Server::run() -> void { _server.run(); }
}  // namespace Websockets