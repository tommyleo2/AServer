//
// Created by tommy on 4/20/19.
//

#ifndef ASERVER_TCP_SERVER_HPP
#define ASERVER_TCP_SERVER_HPP

#include "core/io_server.hpp"

namespace AServer::Framework {
class TCPServer : public Core::IOServer {
  public:
    TCPServer();
    ~TCPServer() override = default;
    void onStart() override;
    void onEnd() override {}
};
}  // namespace AServer::Framework

#endif  // ASERVER_TCP_SERVER_HPP
