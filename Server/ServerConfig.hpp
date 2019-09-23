//
// Created by alex on 9/23/19.
//

#ifndef BABEL_SERVER_SERVERCONFIG_HPP
#define BABEL_SERVER_SERVERCONFIG_HPP

#include <string>

struct ServerConfig {
    std::string databaseUrl;
    size_t maxUser;
};

#endif //BABEL_SERVER_SERVERCONFIG_HPP
