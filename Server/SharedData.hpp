//
// Created by alex on 9/25/19.
//

#ifndef BABEL_SERVER_SHAREDDATA_HPP
#define BABEL_SERVER_SHAREDDATA_HPP

#include "Database.hpp"
#include "boost/asio/ip/tcp.hpp"

struct SharedData {
    Database& database;
    boost::asio::ip::tcp::socket& socket;
};

#endif //BABEL_SERVER_SHAREDDATA_HPP
