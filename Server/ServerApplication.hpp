//
// Created by alex on 9/23/19.
//

#ifndef BABEL_SERVER_SERVERAPPLICATION_HPP
#define BABEL_SERVER_SERVERAPPLICATION_HPP

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "Database.hpp"
#include "ServerConfig.hpp"
#include "ServerError.hpp"
#include "Session.hpp"
#include "SharedData.hpp"
#include <memory>
#include <list>

typedef boost::asio::ip::tcp BoostTcp;

class Session;

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
    std::list<boost::shared_ptr<Session>> sessions_;

    // Private logic
    void handleAccept(const boost::shared_ptr<Session>& session, const boost::system::error_code& ec);
};

#endif //BABEL_SERVER_SERVERAPPLICATION_HPP
