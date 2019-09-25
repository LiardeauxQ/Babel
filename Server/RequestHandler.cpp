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
    case CLIENT_GOODBYE:
        goodbye((client_goodbye_t*)request.payload());
    case CLIENT_REGISTER:
        clientRegister((client_register_t*)request.payload());
    default:
        throw "Unknown request.";
    }
}

void RequestHandler::ping(client_ping_t* payload)
{
}

void RequestHandler::hello(client_hello_t* payload)
{
}

void RequestHandler::friendRequest(client_friend_request_t* payload)
{
}

void RequestHandler::goodbye(client_goodbye_t* payload)
{
}

void RequestHandler::clientRegister(client_register_t* payload)
{
}

void RequestHandler::call(client_call_t* payload)
{
}

void RequestHandler::bye(client_bye_t*)
{
}

void RequestHandler::acceptFriend(client_accept_friend_t* payload)
{
}

void RequestHandler::friendStatus(client_friend_status_t*)
{
}
