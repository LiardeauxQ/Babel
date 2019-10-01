//
// Created by Quentin Liardeaux on 9/27/19.
//

#include "ServerRequest.hpp"

ServerRequest::ServerRequest(const std::string &ipAddress, int port) :
    context_()
    , socket_(BoostTcp::socket(context_))
{
    socket_.connect(BoostTcp::endpoint(boost::asio::ip::address::from_string(ipAddress), port));
}

ServerRequest::~ServerRequest()
{
    socket_.close();
}

void ServerRequest::callbackWrite(const boost::system::error_code& ec)
{
    if (ec)
        std::cerr << ec.message() << std::endl;
}

void ServerRequest::sendPing(time_t timeSent)
{
    client_ping_t clt = {timeSent};
    Message message(CLIENT_PING, CLIENT_PING_SIZE, &clt);

    boost::asio::async_write(
            socket_,
            boost::asio::buffer(message.getHeaderRaw(), HEADER_SIZE),
            boost::bind(callbackWrite, boost::asio::placeholders::error));
    boost::asio::async_write(
            socket_,
            boost::asio::buffer(message.getPayload(), message.getPayloadSize()),
            boost::bind(callbackWrite, boost::asio::placeholders::error));
}

void ServerRequest::sendHello(const std::string &username, const std::string &password)
{
    client_hello_t clt;

    if (username.size() < USERNAME_LEN)
        strcpy(clt.username, username.c_str());
    if (password.size() < PASSWORD_LEN)
        strcpy(clt.password, password.c_str());

    Message message(CLIENT_HELLO, CLIENT_HELLO_SIZE, &clt);

    boost::asio::async_write(
            socket_,
            boost::asio::buffer(message.getData(), message.getTotalSize()),
            boost::bind(callbackWrite, boost::asio::placeholders::error));
}

void ServerRequest::sendGoodbye()
{

}

void ServerRequest::sendFriendStatus()
{

}

void ServerRequest::sendRegister(const std::string &username, const std::string &password)
{

}

void ServerRequest::sendFriendRequest(const std::string &username)
{

}

void ServerRequest::sendCall(const std::vector<std::string> &usernames)
{

}

void ServerRequest::sendBye()
{

}

void ServerRequest::sendAccecptFriend(const std::string &message)
{

}

std::string ServerRequest::receiveFriendRequest()
{

}

void ServerRequest::receiveCall()
{

}

void ServerRequest::receiveBye()
{

}