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

#include "../NotificationHandler.hpp"
#include "ServerResponse.hpp"
#include "Message.hpp"

typedef boost::asio::ip::tcp BoostTcp;

class ServerCommunication {
public:
    ServerCommunication(const std::string &ipAddress, int port);
    ~ServerCommunication();

    void start();
    void stop();

    void sendRequest(boost::shared_ptr<boost::mutex> mutex, boost::shared_ptr<std::queue<Message>> queue);
    void receiveResponse(ServerResponse *response, boost::shared_ptr<boost::mutex> mutex, boost::shared_ptr<std::queue<Message>> queue);
private:

    void write(Message &message);
    Message read();

    boost::asio::io_context context_;
    BoostTcp::socket socket_;
    std::string ipAddress_;
    int port_;
};


#endif //BABEL_SERVER_SERVERCOMMUNICATION_HPP
