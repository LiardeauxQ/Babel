//
// Created by Quentin Liardeaux on 9/27/19.
//

#include "ServerRequest.hpp"

boost::shared_ptr<ServerRequest> ServerRequest::create(const std::string &ipAddress, int port, boost::shared_ptr<NotificationHandler> notifHandler)
{
    return boost::shared_ptr<ServerRequest>(new ServerRequest(ipAddress, port, notifHandler));
}

ServerRequest::ServerRequest(const std::string &ipAddress, int port, boost::shared_ptr<NotificationHandler> notifHandler) :
    notifHandler_(notifHandler)
    , context_()
    , socket_(BoostTcp::socket(context_))
    , ipAddress_(ipAddress)
    , port_(port)
{
    subjects_.push_back(boost::shared_ptr<Subject>(new Subject("loginResponse")));
    subjects_.push_back(boost::shared_ptr<Subject>(new Subject("registerResponse")));
}

ServerRequest::~ServerRequest()
{
    socket_.close();
}

void ServerRequest::start()
{
    socket_.connect(BoostTcp::endpoint(
            boost::asio::ip::address::from_string(ipAddress_), port_));
    context_.run();
}

void ServerRequest::sendPing(time_t timeSent)
{
    client_ping_t clt = {timeSent};
    Message message(CLIENT_PING, CLIENT_PING_SIZE, &clt);

    sendData(message);
}

void ServerRequest::pingResponse(server_ping_response_t *srv)
{

}

void ServerRequest::sendHello(const std::string &username, const std::string &password)
{
    client_hello_t clt;

    if (username.size() < USERNAME_LEN)
        strcpy(clt.username, username.c_str());
    if (password.size() < PASSWORD_LEN)
        strcpy(clt.password, password.c_str());

    Message message(CLIENT_HELLO, CLIENT_HELLO_SIZE, &clt);

    sendData(message);
}

void ServerRequest::helloResponse(server_hello_response_t *srv)
{
    std::map<std::string, void*> userInfo;

    userInfo.insert(std::pair<std::string, void*>(std::string("type"),
            (void*)(std::string("result").c_str())));
    userInfo.insert(std::pair<std::string, void*>(std::string("result"),
            (void*)(&srv->result)));
    for (auto sub : subjects_) {
        if (sub->getLabel() == "loginResponse") {
            sub->notify(userInfo);
            break;
        }
    }
}

void ServerRequest::sendGoodbye()
{
    client_goodbye_t clt;

    Message message(CLIENT_GOODBYE, CLIENT_GOODBYE_SIZE, &clt);

    sendData(message);
}

void ServerRequest::sendFriendStatus()
{
    client_friend_status_t clt;
    Message message(CLIENT_FRIEND_STATUS, CLIENT_FRIEND_STATUS_SIZE, &clt);

    sendData(message);
}

void ServerRequest::sendRegister(const std::string &username, const std::string &password)
{
    client_register_t clt;

    if (username.size() < USERNAME_LEN)
        strcpy(clt.username, username.c_str());
    if (password.size() < PASSWORD_LEN)
        strcpy(clt.password, password.c_str());

    Message message(CLIENT_REGISTER, CLIENT_REGISTER_SIZE, &clt);

    sendData(message);
}

void ServerRequest::registerResponse(server_register_response_t *srv)
{
    std::map<std::string, void*> userInfo;

    userInfo.insert(std::pair<std::string, void*>(std::string("type"),
            (void*)(std::string("result").c_str())));
    userInfo.insert(std::pair<std::string, void*>(std::string("result"),
            (void*)(&srv->result)));
    for (auto sub : subjects_) {
        if (sub->getLabel() == "registerResponse") {
            sub->notify(userInfo);
            break;
        }
    }
}

void ServerRequest::sendFriendRequest(const std::string &username)
{
    client_friend_request_t clt;

    if (username.size() < USERNAME_LEN)
        strcpy(clt.username, username.c_str());

    Message message(CLIENT_FRIEND_REQUEST, CLIENT_FRIEND_REQUEST_SIZE, &clt);

    sendData(message);
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

    sendData(message);
}

void ServerRequest::callResponse(server_call_response_t *srv)
{

}

void ServerRequest::sendBye()
{
    client_bye_t clt;

    Message message(CLIENT_BYE, CLIENT_BYE_SIZE, &clt);

    sendData(message);
}

void ServerRequest::byeResponse(server_bye_response_t *srv)
{

}

void ServerRequest::sendAccecptFriend(const std::string &message)
{
    client_accept_friend_t clt;

    if (message.size() < MESSAGE_LEN)
        strcpy(clt.message, message.c_str());

    Message sendingMsg(CLIENT_ACCEPT_FRIEND, CLIENT_ACCEPT_FRIEND_SIZE, &clt);

    sendData(sendingMsg);
}

void ServerRequest::sendData(Message &message)
{
    boost::asio::async_write(
            socket_,
            boost::asio::buffer(message.getData(), message.getTotalSize()),
            boost::bind(&ServerRequest::waitForResponse, shared_from_this(), boost::asio::placeholders::error));
}

void ServerRequest::waitForResponse(const boost::system::error_code &ec)
{
    std::cout << "waiting" << std::endl;
    boost::asio::async_read(
            socket_,
            boost::asio::buffer(response_.getHeaderRaw(), HEADER_SIZE),
            boost::bind(&ServerRequest::receivePacket, shared_from_this(), boost::asio::placeholders::error));
}

void ServerRequest::receivePacket(const boost::system::error_code &ec)
{
    if (!ec) {
        std::cout << "packet" << std::endl;
        response_.setupPayload();
        boost::asio::async_read(
                socket_,
                boost::asio::buffer(response_.getPayload(), response_.getPayloadSize()),
                boost::bind(&ServerRequest::receiveBody, shared_from_this(), boost::asio::placeholders::error));
    } else {
        std::cerr << "Error while receiving packet." << ec.message() << std::endl;
    }
}

void ServerRequest::receiveBody(const boost::system::error_code &ec)
{
    if (!ec) {
        handleResponse(response_);
    } else {
        std::cerr  << "Error while receiving body." << ec.message() << std::endl;
    }
}

void ServerRequest::handleResponse(Message &response)
{
    std::cout << "Response" << std::endl;
    switch (response.getId()) {
        case SERVER_PING_RESPONSE:
            pingResponse((server_ping_response_t*)(response.getPayload()));
            break;
        case SERVER_HELLO_RESPONSE:
            helloResponse((server_hello_response_t*)(response.getPayload()));
            break;
        case SERVER_GOODBYE_RESPONSE:
            byeResponse((server_bye_response_t*)(response.getPayload()));
            break;
        case SERVER_REGISTER_RESPONSE:
            registerResponse((server_register_response_t*)(response.getPayload()));
            break;
        case SERVER_CALL_RESPONSE:
            callResponse((server_call_response_t*)(response.getPayload()));
            break;
        default:
            break;
    }
}

std::string ServerRequest::receiveFriendRequest()
{
    return std::string("");
}

void ServerRequest::receiveCall()
{

}

void ServerRequest::receiveBye()
{

}
