//
// Created by alex on 9/23/19.
//

#ifndef BABEL_SERVER_SERVERAPPLICATION_HPP
#define BABEL_SERVER_SERVERAPPLICATION_HPP

#include <boost/asio.hpp>

class ServerApplication {
public:
    ServerApplication() = default;
    ~ServerApplication() = default;

    ServerApplication(const ServerApplication&) = delete;
private:

    std::vector<User>
};

#endif //BABEL_SERVER_SERVERAPPLICATION_HPP
