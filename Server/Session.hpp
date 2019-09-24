//
// Created by alex on 9/24/19.
//

#ifndef BABEL_SERVER_SESSION_HPP
#define BABEL_SERVER_SESSION_HPP

#include <boost/asio.hpp>
#include <iostream>

typedef boost::asio::ip::tcp BoostTcp;

class Session {
public:
    static std::shared_ptr<Session> create(boost::asio::io_context& context);

    [[nodiscard]] BoostTcp::socket& getSocket()
    {
        return socket_;
    }

    void run();

private:

    explicit Session(boost::asio::io_context& context);

    BoostTcp::socket socket_;
};

#endif //BABEL_SERVER_SESSION_HPP
