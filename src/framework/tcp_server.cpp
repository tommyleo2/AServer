//
// Created by tommy on 4/20/19.
//

#include "framework/tcp_server.hpp"
#include "core/weak_call.hpp"

using namespace AServer::Framework;

TCPServer::TCPServer() : IOServer() {}

struct Counter {
    Counter() : counter(0) {
        std::cout << "Con" << std::endl;
    }
    Counter(const Counter &c) : counter(c.counter) {
        std::cout << "Ref" << std::endl;
    }
    Counter(Counter &&c) noexcept : counter(c.counter) {
        std::cout << "Move" << std::endl;
    }
    ~Counter() {
        std::cout << "Des" << std::endl;
    }

    int counter;
};

Counter c;

void work(Counter &c) {
    std::cout << "hello world " << c.counter << std::endl;
    c.counter++;
}

void stop(std::weak_ptr<AServer::Core::TimerHandle> &handle) {
    std::cout << c.counter << std::endl;
    handle.lock()->cancel();
}

void TCPServer::onStart() {
    auto handle = this->getTimer()->repeat(0.5, &::work, std::ref(c));

    this->getTimer()->once(1.2, &::stop, std::move(handle));
}
