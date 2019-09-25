//
// Created by alex on 9/25/19.
//

#ifndef BABEL_SERVER_REQUESTHANDLER_HPP
#define BABEL_SERVER_REQUESTHANDLER_HPP

#include "protocol.h"
#include "Message.hpp"

class RequestHandler {
public:
    RequestHandler() = delete;
    RequestHandler(const RequestHandler&) = delete;
    ~RequestHandler() = delete;

    static void handleRequest(Message& request);

    static void friendRequest(client_friend_request_t *payload);

    static void ping(client_ping_t *payload);

    static void hello(client_hello_t *payload);

    static void goodbye(client_goodbye_t *payload);

    static void clientRegister(client_register_t *payload);

    static void call(client_call_t *payload);

    static void bye(client_bye_t *);

    static void acceptFriend(client_accept_friend_t *payload);

    static void friendStatus(client_friend_status_t *);
};

#endif //BABEL_SERVER_REQUESTHANDLER_HPP
