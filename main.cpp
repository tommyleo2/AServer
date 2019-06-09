#include <iostream>
#include "core/io_server.hpp"

class TestServer : public AServer::Core::IOServer {
  public:
    void onStart() override {
        auto handle = this->getTimer()->repeat(
            0.5, []() { std::cout << "Hello world" << std::endl; });
        this->getTimer()->once(
            2.1, [handle(std::move(handle))]() { handle.lock()->cancel(); });
    }

    void onEnd() override {}
};

int main() {
    TestServer server;
    server.run();
}
