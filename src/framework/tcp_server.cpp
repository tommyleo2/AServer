//
// Created by tommy on 4/20/19.
//

#include "framework/tcp_server.hpp"

using namespace AServer::Framework;

TCPServer::TCPServer() : IOServer() {}

void TCPServer::onStart() {
    auto handle = this->getTimer()->repeat(
        0.5, []() { std::cout << "Hello world" << std::endl; });

    this->getTimer()->once(2, [this, handle]() { handle.lock()->cancel(); });
}
