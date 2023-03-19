#include "Server.hpp"

#include <Logger.hpp>

namespace Websockets {
    using uWS::WebSocket;

    using Logger     = Logger::Quill;
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
                    .listen(SERVER_LISTENING_PORT, [](auto* listenSocket) {
                        if (listenSocket)
                        {
                            LOG_INFO(Logger::getLogger(), "Websocket server listening on port {}", SERVER_LISTENING_PORT);
                        } else {
                            LOG_ERROR(Logger::getLogger(), "Websocket failed to bind to port {}", SERVER_LISTENING_PORT);
                        }
                    })) {}

    auto Server::publish(std::string_view topic, std::string_view data, OpCode code, bool compress) -> bool {
        LOG_DEBUG(Logger::getLogger(), "Websocket server send `{}` on topic `{}`", topic, data);

        return _server.publish(topic, data, code, compress);
    }

    auto Server::run() -> void { _server.run(); }
}  // namespace Websockets