//
// Created by alex on 9/25/19.
//

#include "RequestHandler.hpp"

void RequestHandler::handleRequest(Message& request)
{
    switch (request.getId()) {
    case CLIENT_HELLO:
        hello((client_hello_t*)request.payload());
    case CLIENT_PING:
        ping((client_ping_t*)request.payload());
    case CLIENT_ACCEPT_FRIEND:
        acceptFriend((client_accept_friend_t*)request.payload());
    case CLIENT_FRIEND_REQUEST:
        friendRequest((client_friend_request_t*)request.payload());
    case CLIENT_BYE:
        bye((client_bye_t*)request.payload());
    case CLIENT_CALL:
        call((client_call_t*)request.payload());
    default:
        throw "Unknown request.";
    }
}
