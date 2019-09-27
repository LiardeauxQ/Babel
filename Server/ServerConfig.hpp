//
// Created by alex on 9/23/19.
//

#ifndef BABEL_SERVER_SERVERCONFIG_HPP
#define BABEL_SERVER_SERVERCONFIG_HPP

#include <iostream>
#include <fstream>
#include <string>

static const char* DEFAULT_DB = "data.db";
static const char* DEFAULT_CONFIG_PATH = "server.cfg";
static const size_t DEFAULT_MAX_USER = 100;
static const size_t DEFAULT_PORT = 8080;
static const char* DEFAULT_HOST = "0.0.0.0";

struct ServerConfig {
public:
    ServerConfig(const char* path);

    ServerConfig();

    std::string databaseUrl;
    size_t maxUser;
    short port;
    std::string host;

private:
    void parseFile(std::ifstream& file);
    std::string configPath_;
};

static std::ostream& operator<<(std::ostream& os, const ServerConfig& config)
{
    os << "ServerConfiguration {" << std::endl;
    os << "  databaseUrl: " << config.databaseUrl << std::endl;
    os << "  port: " << config.port << std::endl;
    os << "  max_user: " << config.maxUser << std::endl;
    os << "}" << std::endl;
    return os;
}

#endif //BABEL_SERVER_SERVERCONFIG_HPP
