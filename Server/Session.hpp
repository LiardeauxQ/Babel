//
// Created by alex on 9/24/19.
//

#ifndef BABEL_SERVER_SESSION_HPP
#define BABEL_SERVER_SESSION_HPP

#include "RequestHandler.hpp"
#include "Message.hpp"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>
#include <vector>

typedef boost::asio::ip::tcp BoostTcp;

class Session : public boost::enable_shared_from_this<Session> {
public:
    static boost::shared_ptr<Session> create(boost::asio::io_context& context);

    [[nodiscard]] BoostTcp::socket& getSocket()
    {
        return socket_;
    }

    void run(std::vector<boost::shared_ptr<Session>>& sessions);

    void receivePacket(const boost::system::error_code& ec);

    void receiveBody(const boost::system::error_code& ec);

private:
    explicit Session(boost::asio::io_context& context);

    BoostTcp::socket socket_;

    Message request_;

    union {
        char raw[REQUEST_SIZE];
        request_t req;
    } data_;
};

#endif //BABEL_SERVER_SESSION_HPP
