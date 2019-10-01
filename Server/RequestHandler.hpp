//
// Created by alex on 9/25/19.
//

#ifndef BABEL_SERVER_REQUESTHANDLER_HPP
#define BABEL_SERVER_REQUESTHANDLER_HPP

#include "protocol.h"
#include "SharedData.hpp"
#include "Message.hpp"

class RequestHandler {
public:
    RequestHandler() = delete;
    RequestHandler(const RequestHandler&) = delete;
    ~RequestHandler() = delete;

    static void handleRequest(Message& request, SharedData& data);

    void friendRequest(client_friend_request_t *payload, SharedData& data);

    void ping(client_ping_t *payload, SharedData& data);

    void hello(client_hello_t *payload, SharedData& data);

    void goodbye(client_goodbye_t *payload, SharedData& data);

    void clientRegister(client_register_t *payload, SharedData& data);

    void call(client_call_t *payload, SharedData& data);

    void bye(client_bye_t *, SharedData& data);

    void acceptFriend(client_accept_friend_t *payload, SharedData& data);

    void friendStatus(client_friend_status_t *, SharedData& data);
};

#endif //BABEL_SERVER_REQUESTHANDLER_HPP
