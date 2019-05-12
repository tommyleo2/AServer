//
// Created by tommy on 4/20/19.
//

#include "core/io_server.hpp"

using namespace AServer::Core;
using namespace boost::asio;

IOServer::IOServer() : m_io(), m_timer(Timer::create(m_io)) {}

void IOServer::run() {
    this->getTimer()->once(0, std::mem_fn(&IOServer::onStart), this);
    m_io.run();
    this->onEnd();
}
