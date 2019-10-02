//
// Created by alex on 9/23/19.
//

#ifndef BABEL_SERVER_SERVERCONFIG_HPP
#define BABEL_SERVER_SERVERCONFIG_HPP

#include <iostream>
#include <fstream>
#include <string>


struct ServerConfig {
public:
    explicit ServerConfig(const char* path = "server.cfg");

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
