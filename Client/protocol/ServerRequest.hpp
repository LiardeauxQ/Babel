//
// Created by Quentin Liardeaux on 9/27/19.
//

#ifndef BABEL_SERVER_SERVERREQUEST_HPP
#define BABEL_SERVER_SERVERREQUEST_HPP

#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>
#include <time.h>

#include "Message.hpp"
#include "protocol.h"
#include "../NotificationHandler.hpp"

typedef boost::asio::ip::tcp BoostTcp;

class ServerRequest
    : public boost::enable_shared_from_this<ServerRequest> {
public:
    ServerRequest(const std::string &ipAddress, int port,
            boost::shared_ptr<NotificationHandler> notifHandler);

    ~ServerRequest();

    static boost::shared_ptr<ServerRequest> create(const std::string &ipAddress, int port,
            boost::shared_ptr<NotificationHandler> notifHandler);

    void start();

    void sendPing(time_t timeSent);
    void sendHello(const std::string &username, const std::string &password);
    void sendGoodbye();
    void sendFriendStatus();
    void sendRegister(const std::string &username, const std::string &password);
    void sendFriendRequest(const std::string &username);
    void sendCall(const std::vector<std::string> &usernames);
    void sendBye();
    void sendAccecptFriend(const std::string &message);

    std::string receiveFriendRequest();
    void receiveCall();
    void receiveBye();

private:
    void pingResponse(server_ping_response_t *srv);
    void helloResponse(server_hello_response_t *srv);
    void registerResponse(server_register_response_t *srv);
    void callResponse(server_call_response_t *srv);
    void byeResponse(server_bye_response_t *srv);

    void sendData(Message &message);
    void waitForResponse(const boost::system::error_code &ec);
    void receivePacket(const boost::system::error_code &ec);
    void receiveBody(const boost::system::error_code &ec);
    void handleResponse(Message &message);

    boost::shared_ptr<NotificationHandler> notifHandler_;
    boost::asio::io_context context_;
    BoostTcp::socket socket_;
    std::string ipAddress_;
    Message response_;
    int port_;
    std::vector<boost::shared_ptr<Subject>> subjects_;
};


#endif //BABEL_SERVER_SERVERREQUEST_HPP
