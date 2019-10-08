//
// Created by Quentin Liardeaux on 10/4/19.
//

#ifndef BABEL_SERVER_SERVERHANDLER_HPP
#define BABEL_SERVER_SERVERHANDLER_HPP

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <queue>
#include <map>

#include "../NotificationHandler.hpp"
#include "ServerCommunication.hpp"
#include "ServerRequest.hpp"
#include "ServerResponse.hpp"
#include "protocol.h"

class ServerHandler {
public:
    ServerHandler(const std::string &ipAddress, int port, boost::shared_ptr<NotificationHandler> notifHandler);
    ~ServerHandler();

    void start();
    void stop();

    void send(int id, std::map<std::string, void*> userInfo);

    const std::string &getIpAddress() const { return ipAddress_; }
    int getPort() const { return port_; }

    static boost::asio::ip::address getLocalIpAddress();
private:
    boost::shared_ptr<NotificationHandler> notifHandler_;
    ServerCommunication communicationHandler_;
    ServerRequest requestsHandler_;
    boost::thread requestsThread_;
    boost::thread responsesThread_;
    boost::shared_ptr<boost::mutex> requestsMutex_;
    boost::shared_ptr<std::queue<Message>> requests_;
    boost::shared_ptr<boost::mutex> responsesMutex_;
    boost::shared_ptr<std::queue<Message>> responses_;
    ServerResponse *responsesHandler_;
    std::string ipAddress_;
    int port_;
};

#endif //BABEL_SERVER_SERVERHANDLER_HPP
