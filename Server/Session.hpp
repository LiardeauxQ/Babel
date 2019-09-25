//
// Created by alex on 9/24/19.
//

#ifndef BABEL_SERVER_SESSION_HPP
#define BABEL_SERVER_SESSION_HPP

#include "Message.hpp"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
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

    void receivePacket(const boost::system::error_code& ec);

    void receiveBody(const boost::system::error_code& ec);

private:
    explicit Session(boost::asio::io_context& context);

    BoostTcp::socket socket_;

    Message request_;
};

#endif //BABEL_SERVER_SESSION_HPP
