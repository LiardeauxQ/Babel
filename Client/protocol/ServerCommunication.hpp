//
// Created by Quentin Liardeaux on 10/4/19.
//

#ifndef BABEL_SERVER_SERVERCOMMUNICATION_HPP
#define BABEL_SERVER_SERVERCOMMUNICATION_HPP

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#include <queue>
#include <string>
#include <unistd.h>

#include "../NotificationHandler.hpp"
#include "Message.hpp"

typedef boost::asio::ip::tcp BoostTcp;

class ServerCommunication {
public:
    ServerCommunication(const std::string &ipAddress, int port);
    ~ServerCommunication();

    void start();

    void sendRequest(boost::shared_ptr<boost::mutex> mutex, boost::shared_ptr<std::queue<Message>> queue);
private:

    void write(Message &message);

    boost::asio::io_context context_;
    BoostTcp::socket socket_;
    std::string ipAddress_;
    int port_;
};


#endif //BABEL_SERVER_SERVERCOMMUNICATION_HPP
