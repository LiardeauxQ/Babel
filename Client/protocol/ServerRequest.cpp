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
            boost::asio::buffer(message.getData(), message.getTotalSize()),
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
    client_goodbye_t clt;

    Message message(CLIENT_GOODBYE, CLIENT_GOODBYE_SIZE, &clt);

    boost::asio::async_write(
            socket_,
            boost::asio::buffer(message.getData(), message.getTotalSize()),
            boost::bind(callbackWrite, boost::asio::placeholders::error));
}

void ServerRequest::sendFriendStatus()
{
    client_friend_status_t clt;

    Message message(CLIENT_FRIEND_STATUS, CLIENT_FRIEND_STATUS_SIZE, &clt);

    boost::asio::async_write(
            socket_,
            boost::asio::buffer(message.getData(), message.getTotalSize()),
            boost::bind(callbackWrite, boost::asio::placeholders::error));
}

void ServerRequest::sendRegister(const std::string &username, const std::string &password)
{
    client_register_t clt;

    if (username.size() < USERNAME_LEN)
        strcpy(clt.username, username.c_str());
    if (password.size() < PASSWORD_LEN)
        strcpy(clt.password, password.c_str());

    Message message(CLIENT_REGISTER, CLIENT_REGISTER_SIZE, &clt);

    boost::asio::async_write(
            socket_,
            boost::asio::buffer(message.getData(), message.getTotalSize()),
            boost::bind(callbackWrite, boost::asio::placeholders::error));
}

void ServerRequest::sendFriendRequest(const std::string &username)
{
    client_friend_request_t clt;

    if (username.size() < USERNAME_LEN)
        strcpy(clt.username, username.c_str());

    Message message(CLIENT_FRIEND_REQUEST, CLIENT_FRIEND_REQUEST_SIZE, &clt);

    boost::asio::async_write(
            socket_,
            boost::asio::buffer(message.getData(), message.getTotalSize()),
            boost::bind(callbackWrite, boost::asio::placeholders::error));
}

void ServerRequest::sendCall(const std::vector<std::string> &usernames)
{
    client_call_t clt;
    int i = 0;

    for (const auto username : usernames) {
        if (i > MAX_FRIENDS)
            break;
        if (username.size() < USERNAME_LEN)
            strcpy(clt.usernames[i++], username.c_str());
    }

    Message message(CLIENT_CALL, CLIENT_CALL_SIZE, &clt);

    boost::asio::async_write(
            socket_,
            boost::asio::buffer(message.getData(), message.getTotalSize()),
            boost::bind(callbackWrite, boost::asio::placeholders::error));
}

void ServerRequest::sendBye()
{
    client_bye_t clt;

    Message message(CLIENT_BYE, CLIENT_BYE_SIZE, &clt);

    boost::asio::async_write(
            socket_,
            boost::asio::buffer(message.getData(), message.getTotalSize()),
            boost::bind(callbackWrite, boost::asio::placeholders::error));
}

void ServerRequest::sendAccecptFriend(const std::string &message)
{
    client_accept_friend_t clt;

    if (message.size() < MESSAGE_LEN)
        strcpy(clt.message, message.c_str());

    Message sendingMsg(CLIENT_ACCEPT_FRIEND, CLIENT_ACCEPT_FRIEND_SIZE, &clt);

    boost::asio::async_write(
            socket_,
            boost::asio::buffer(sendingMsg.getData(), sendingMsg.getTotalSize()),
            boost::bind(callbackWrite, boost::asio::placeholders::error));
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
