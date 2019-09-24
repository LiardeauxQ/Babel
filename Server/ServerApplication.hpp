//
// Created by alex on 9/23/19.
//

#ifndef BABEL_SERVER_SERVERAPPLICATION_HPP
#define BABEL_SERVER_SERVERAPPLICATION_HPP

#include "Database.hpp"
#include "ServerConfig.hpp"
#include "ServerError.hpp"
#include <boost/asio.hpp>
#include <vector>

typedef boost::asio::ip::tcp BoostTcp;

class ServerApplication {
public:
    explicit ServerApplication(const ServerConfig& config);

    ~ServerApplication() = default;

    ServerApplication(const ServerApplication&) = delete;

    void run();
private:
    Database database_;
    bool running_;

    // Network
    boost::asio::io_context context_;
    BoostTcp::acceptor acceptor_;
    std::vector<BoostTcp::socket> clients_;

    // Private logic
    void doAccept();
};

#endif //BABEL_SERVER_SERVERAPPLICATION_HPP
