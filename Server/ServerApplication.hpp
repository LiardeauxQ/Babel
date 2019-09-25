//
// Created by alex on 9/23/19.
//

#ifndef BABEL_SERVER_SERVERAPPLICATION_HPP
#define BABEL_SERVER_SERVERAPPLICATION_HPP

#include "Database.hpp"
#include "ServerConfig.hpp"
#include "ServerError.hpp"
#include <boost/asio.hpp>

class ServerApplication {
public:
    explicit ServerApplication(const ServerConfig& config)
        : database_(config.databaseUrl)
    {
    }

    ~ServerApplication() = default;

    ServerApplication(const ServerApplication&) = delete;

    Database& getConnection() { return database_; };

private:
    Database database_;
};

#endif //BABEL_SERVER_SERVERAPPLICATION_HPP
