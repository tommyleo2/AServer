//
// Created by tommy on 4/20/19.
//

#ifndef ASERVER_IO_SERVER_HPP
#define ASERVER_IO_SERVER_HPP

#include "boost/asio.hpp"

#include "timer.hpp"

namespace AServer::Core {

class IOServer {
  public:
    IOServer();
    virtual ~IOServer() = default;

    virtual void onStart() = 0;
    virtual void onEnd() = 0;
    void run();

    inline shared_ptr<Timer> getTimer() {
        return m_timer;
    }

    IOServer(const IOServer &) = delete;
    IOServer &operator=(const IOServer &) = delete;

  private:
    boost::asio::io_context m_io;
    shared_ptr<Timer> m_timer;
};

}  // namespace AServer::Core

#endif  // ASERVER_IO_SERVER_HPP
