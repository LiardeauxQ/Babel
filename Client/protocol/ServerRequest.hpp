//
// Created by Quentin Liardeaux on 9/27/19.
//

#ifndef BABEL_SERVER_SERVERREQUEST_HPP
#define BABEL_SERVER_SERVERREQUEST_HPP

#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <time.h>

#include "Message.hpp"
#include "protocol.h"

typedef boost::asio::ip::tcp BoostTcp;

class ServerRequest {
public:
    ServerRequest(const std::string &ipAddress, int port);

    ~ServerRequest();

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

    static void callbackWrite(const boost::system::error_code &ec);
private:

    boost::asio::io_context context_;
    BoostTcp::socket socket_;
};


#endif //BABEL_SERVER_SERVERREQUEST_HPP
