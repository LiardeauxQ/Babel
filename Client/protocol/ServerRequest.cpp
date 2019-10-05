//
// Created by Quentin Liardeaux on 9/27/19.
//

#include "ServerRequest.hpp"

std::vector<std::tuple<int, Message (*)(std::map<std::string, void*>)>> ServerRequest::requests = {
        std::make_tuple(CLIENT_PING, &ServerRequest::ping),
        std::make_tuple(CLIENT_HELLO, &ServerRequest::hello),
        std::make_tuple(CLIENT_GOODBYE, &ServerRequest::goodbye),
        std::make_tuple(CLIENT_FRIEND_STATUS, &ServerRequest::friendStatus),
        std::make_tuple(CLIENT_REGISTER, &ServerRequest::registerRequest),
        std::make_tuple(CLIENT_FRIEND_REQUEST, &ServerRequest::friendRequest),
        std::make_tuple(CLIENT_CALL, &ServerRequest::call),
        std::make_tuple(CLIENT_BYE, &ServerRequest::bye),
        std::make_tuple(CLIENT_ACCEPT_FRIEND, &ServerRequest::acceptFriend),
};

Message ServerRequest::createRequest(int id, std::map<std::string, void*> userInfo)
{
    for (auto request : requests) {
        if (std::get<0>(request) == id)
            return std::get<1>(request)(userInfo);
    }
    throw "Invalid id";
}

Message ServerRequest::ping(std::map<std::string, void*> userInfo)
{
    auto it = userInfo.find("time");

    if (it == userInfo.end())
        throw "Cannot find time";
    time_t time = *(time_t*)(it->second);

    client_ping_t *clt = (client_ping_t*)calloc(CLIENT_PING_SIZE, 1);
    clt->stamp = time;

    Message message(CLIENT_PING, CLIENT_PING_SIZE, clt);

    return message;
}

Message ServerRequest::hello(std::map<std::string, void*> userInfo)
{
    auto userIt = userInfo.find("username");
    auto passIt = userInfo.find("password");

    if (userIt == userInfo.end())
        throw "Cannot find username key";
    else if (passIt == userInfo.end())
        throw "Cannot find password key";

    char *username = (char*)(userIt->second);
    char *password = (char*)(passIt->second);
    client_hello_t *clt = (client_hello_t*)calloc(CLIENT_HELLO_SIZE, 1);

    if (strlen(username) < USERNAME_LEN)
        strcpy(clt->username, username);
    if (strlen(password) < PASSWORD_LEN)
        strcpy(clt->password, password);

    Message message(CLIENT_HELLO, CLIENT_HELLO_SIZE, clt);

    return message;
}

Message ServerRequest::goodbye(std::map<std::string, void*> userInfo)
{
    client_goodbye_t *clt = (client_goodbye_t*)calloc(CLIENT_GOODBYE_SIZE, 1);
    Message message(CLIENT_GOODBYE, CLIENT_GOODBYE_SIZE, clt);

    return message;
}

Message ServerRequest::friendStatus(std::map<std::string, void*> userInfo)
{
    client_friend_status_t *clt = (client_friend_status_t*)calloc(CLIENT_FRIEND_STATUS_SIZE, 1);
    Message message(CLIENT_FRIEND_STATUS, CLIENT_FRIEND_STATUS_SIZE, clt);

    return message;
}

Message ServerRequest::registerRequest(std::map<std::string, void*> userInfo)
{
    auto userIt = userInfo.find("username");
    auto passIt = userInfo.find("password");

    if (userIt == userInfo.end())
        throw "Cannot find username key";
    else if (passIt == userInfo.end())
        throw "Cannot find password key";

    char *username = (char*)(userIt->second);
    char *password = (char*)(passIt->second);

    client_register_t *clt = (client_register_t*)calloc(CLIENT_REGISTER_SIZE, 1);

    if (strlen(username) < USERNAME_LEN)
        strcpy(clt->username, username);
    if (strlen(password) < PASSWORD_LEN)
        strcpy(clt->password, password);

    Message message(CLIENT_REGISTER, CLIENT_REGISTER_SIZE, clt);

    return message;
}

Message ServerRequest::friendRequest(std::map<std::string, void*> userInfo)
{
    auto userIt = userInfo.find("username");

    if (userIt == userInfo.end())
        throw "Cannot find username key";

    char *username = (char*)(userIt->second);
    client_friend_request_t *clt = (client_friend_request_t*)calloc(CLIENT_FRIEND_REQUEST_SIZE, 1);

    if (strlen(username) < USERNAME_LEN)
        strcpy(clt->username, username);

    Message message(CLIENT_FRIEND_REQUEST, CLIENT_FRIEND_REQUEST_SIZE, clt);

    return message;
}

Message ServerRequest::call(std::map<std::string, void*> userInfo)
{
    auto userIt = userInfo.find("username");
    auto addressIpIt = userInfo.find("addressIp");
    auto portIt = userInfo.find("port");

    if (userIt == userInfo.end())
        throw "Cannot find usernames key";
    else if (addressIpIt == userInfo.end())
        throw "Cannot find addressIp key";
    else if (portIt == userInfo.end())
        throw "Cannot find port key";

    char *username = (char*)(userIt->second);
    char *addressIp = (char*)(addressIpIt->second);
    int port = *(int*)portIt->second;
    client_call_t *clt = (client_call_t*)calloc(CLIENT_CALL_SIZE, 1);

    strcpy(clt->usernames[0], username);
    strcpy(clt->ip, addressIp);
    clt->port = port;
    clt->number = 1;

    Message message(CLIENT_CALL, CLIENT_CALL_SIZE, clt);

    return message;
}

Message ServerRequest::bye(std::map<std::string, void*> userInfo)
{
    client_bye_t *clt = (client_bye_t*)calloc(CLIENT_BYE_SIZE, 1);

    Message message(CLIENT_BYE, CLIENT_BYE_SIZE, clt);

    return message;
}

Message ServerRequest::acceptFriend(std::map<std::string, void*> userInfo)
{
    auto messageIt = userInfo.find("message");

    if (messageIt == userInfo.end())
        throw "Cannot find message key";

    char *message = (char*)(messageIt->second);
    client_accept_friend_t *clt = (client_accept_friend_t*)calloc(CLIENT_ACCEPT_FRIEND_SIZE, 1);

    if (strlen(message) < USERNAME_LEN)
        strcpy(clt->message, message);

    Message sendingMsg(CLIENT_ACCEPT_FRIEND, CLIENT_ACCEPT_FRIEND_SIZE, clt);

    return sendingMsg;
}