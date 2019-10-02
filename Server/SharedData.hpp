//
// Created by alex on 9/25/19.
//

#ifndef BABEL_SERVER_SHAREDDATA_HPP
#define BABEL_SERVER_SHAREDDATA_HPP

#include "Database.hpp"
#include "Session.hpp"
#include "boost/asio/ip/tcp.hpp"
#include <list>

class Session;

struct SharedData {
    Database& database;
    boost::asio::ip::tcp::socket& socket;
    std::list<boost::shared_ptr<Session>>& sessions;
};

#endif //BABEL_SERVER_SHAREDDATA_HPP
