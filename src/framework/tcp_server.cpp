//
// Created by tommy on 4/20/19.
//

#include "framework/tcp_server.hpp"

using namespace AServer::Framework;

TCPServer::TCPServer() : IOServer() {}

void test(std::vector<int> &t, int a) {
    t.push_back(1);
    std::cout << "TEST " << t.size() << a << std::endl;
}

void stop(const std::weak_ptr<AServer::Core::TimerHandle> &handle) {
    handle.lock()->cancel();
}

void TCPServer::onStart() {
    std::vector<int> a;
    auto handle = this->getTimer()->repeat(0.5, &test, std::move(a), 1);

    this->getTimer()->once(1.2,
                           [this](decltype(handle) &&handle) {
                               this->getTimer()->once(0, &stop, handle);
                           },
                           std::move(handle));
}
