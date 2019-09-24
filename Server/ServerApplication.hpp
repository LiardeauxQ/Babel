//
// Created by alex on 9/23/19.
//

#ifndef BABEL_SERVER_SERVERAPPLICATION_HPP
#define BABEL_SERVER_SERVERAPPLICATION_HPP

#include "Database.hpp"
#include "ServerConfig.hpp"
#include "ServerError.hpp"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <vector>
#include <memory>
#include "Session.hpp"

typedef boost::asio::ip::tcp BoostTcp;

class ServerApplication {
public:
    explicit ServerApplication(const ServerConfig& config);

    ~ServerApplication() = default;

    ServerApplication(const ServerApplication&) = delete;

    void run();
private:
    void accept();

    Database database_;
    bool running_;

    // Network
    boost::asio::io_context context_;
    BoostTcp::acceptor acceptor_;
    std::vector<BoostTcp::socket> clients_;

    // Private logic
    void handleAccept(std::shared_ptr<Session> session, const boost::system::error_code& ec);
    void handleWrite(const boost::system::error_code& ec, size_t byte_trans);
};

#endif //BABEL_SERVER_SERVERAPPLICATION_HPP
