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
#include "protocol.h"

class ServerHandler {
public:
    ServerHandler(const std::string &ipAddress, int port, boost::shared_ptr<NotificationHandler> notifHandler);
    ~ServerHandler();

    void start();

    void *send(int id, std::map<std::string, void*> userInfo);
    void *receive();
private:
    boost::shared_ptr<NotificationHandler> notifHandler_;
    ServerCommunication communicationHandler_;
    ServerRequest requestHandler_;
    boost::thread requestThread_;
    boost::shared_ptr<boost::mutex> requestMutex_;
    boost::shared_ptr<std::queue<Message>> requests_;
    boost::thread responseThread_;
    boost::shared_ptr<boost::mutex> responseMutex_;
    boost::shared_ptr<std::queue<Message>> responses_;
};

#endif //BABEL_SERVER_SERVERHANDLER_HPP
