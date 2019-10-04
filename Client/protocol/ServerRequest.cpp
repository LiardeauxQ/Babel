//
// Created by Quentin Liardeaux on 9/27/19.
//

#include "ServerRequest.hpp"

static std::vector<std::tuple<int, Message (*)(std::map<std::string, void*>)>> requests = {
        std::make_tuple(CLIENT_PING, &ServerRequest::createPingRequest),
        std::make_tuple(CLIENT_HELLO, &ServerRequest::createHelloRequest),
        std::make_tuple(CLIENT_GOODBYE, &ServerRequest::createGoodbyeRequest),
        std::make_tuple(CLIENT_FRIEND_STATUS, &ServerRequest::createFriendStatusRequest),
        std::make_tuple(CLIENT_REGISTER, &ServerRequest::createRegisterRequest),
        std::make_tuple(CLIENT_FRIEND_REQUEST, &ServerRequest::createFriendRequest),
        std::make_tuple(CLIENT_CALL, &ServerRequest::createCallRequest),
        std::make_tuple(CLIENT_BYE, &ServerRequest::createByeRequest),
        std::make_tuple(CLIENT_ACCEPT_FRIEND, &ServerRequest::createAcceptFriendRequest),
};

Message ServerRequest::createRequest(int id, std::map<std::string, void*> userInfo)
{
    for (auto request : requests) {
        if (std::get<0>(request) == id)
            return std::get<1>(request)(userInfo);
    }
    throw "Invalid id";
}

Message ServerRequest::createPingRequest(std::map<std::string, void*> userInfo)
{
    auto it = userInfo.find("time");

    if (it == userInfo.end())
        throw "Cannot find time";
    time_t time = *(time_t*)(it->second);

    client_ping_t clt = {time};
    Message message(CLIENT_PING, CLIENT_PING_SIZE, &clt);

    return message;
}

Message ServerRequest::createHelloRequest(std::map<std::string, void*> userInfo)
{
    auto userIt = userInfo.find("username");
    auto passIt = userInfo.find("password");

    if (userIt == userInfo.end())
        throw "Cannot find username key";
    else if (passIt == userInfo.end())
        throw "Cannot find password key";

    char *username = (char*)(userIt->second);
    char *password = (char*)(passIt->second);
    client_hello_t clt;

    if (strlen(username) < USERNAME_LEN)
        strcpy(clt.username, username);
    if (strlen(password) < PASSWORD_LEN)
        strcpy(clt.password, password);

    Message message(CLIENT_HELLO, CLIENT_HELLO_SIZE, &clt);

    return message;
}

Message ServerRequest::createGoodbyeRequest(std::map<std::string, void*> userInfo)
{
    client_goodbye_t clt;
    Message message(CLIENT_GOODBYE, CLIENT_GOODBYE_SIZE, &clt);

    return message;
}

Message ServerRequest::createFriendStatusRequest(std::map<std::string, void*> userInfo)
{
    client_friend_status_t clt;
    Message message(CLIENT_FRIEND_STATUS, CLIENT_FRIEND_STATUS_SIZE, &clt);

    return message;
}

Message ServerRequest::createRegisterRequest(std::map<std::string, void*> userInfo)
{
    auto userIt = userInfo.find("username");
    auto passIt = userInfo.find("password");

    if (userIt == userInfo.end())
        throw "Cannot find username key";
    else if (passIt == userInfo.end())
        throw "Cannot find password key";

    char *username = (char*)(userIt->second);
    char *password = (char*)(passIt->second);
    client_register_t clt;

    if (strlen(username) < USERNAME_LEN)
        strcpy(clt.username, username);
    if (strlen(password) < PASSWORD_LEN)
        strcpy(clt.password, password);

    Message message(CLIENT_REGISTER, CLIENT_REGISTER_SIZE, &clt);

    return message;
}

Message ServerRequest::createFriendRequest(std::map<std::string, void*> userInfo)
{
    auto userIt = userInfo.find("username");

    if (userIt == userInfo.end())
        throw "Cannot find username key";

    char *username = (char*)(userIt->second);
    client_friend_request_t clt;

    if (strlen(username) < USERNAME_LEN)
        strcpy(clt.username, username);

    Message message(CLIENT_FRIEND_REQUEST, CLIENT_FRIEND_REQUEST_SIZE, &clt);

    return message;
}

Message ServerRequest::createCallRequest(std::map<std::string, void*> userInfo)
{
    auto usersIt = userInfo.find("usernames");
    auto countIt = userInfo.find("count");

    if (usersIt == userInfo.end())
        throw "Cannot find usernames key";
    else if (countIt == userInfo.end())
        throw "Cannot find count key";

    char **usernames = (char**)(usersIt->second);
    int count = *(int*)(countIt->second);
    client_call_t clt;

    for (int i = 0 ; i < count && i < MAX_FRIENDS ; i++) {
        if (strlen(usernames[i]) < USERNAME_LEN)
            strcpy(clt.usernames[i++], usernames[i]);
    }

    Message message(CLIENT_CALL, CLIENT_CALL_SIZE, &clt);

    return message;
}

Message ServerRequest::createByeRequest(std::map<std::string, void*> userInfo)
{
    client_bye_t clt;

    Message message(CLIENT_BYE, CLIENT_BYE_SIZE, &clt);

    return message;
}

Message ServerRequest::createAcceptFriendRequest(std::map<std::string, void*> userInfo)
{
    auto messageIt = userInfo.find("message");

    if (messageIt == userInfo.end())
        throw "Cannot find message key";

    char *message = (char*)(messageIt->second);
    client_accept_friend_t clt;

    if (strlen(message) < USERNAME_LEN)
        strcpy(clt.message, message);

    Message sendingMsg(CLIENT_ACCEPT_FRIEND, CLIENT_ACCEPT_FRIEND_SIZE, &clt);

    return sendingMsg;
}