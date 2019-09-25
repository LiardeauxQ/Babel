//
// Created by alex on 9/25/19.
//

#include "RequestHandler.hpp"
#include <boost/bind.hpp>

void RequestHandler::handleRequest(Message& request, SharedData& data)
{
    switch (request.getId()) {
    case CLIENT_HELLO:
        hello((client_hello_t*)request.getPayload(), data);
        break;
    case CLIENT_PING:
        ping((client_ping_t*)request.getPayload(), data);
        break;
    case CLIENT_ACCEPT_FRIEND:
        acceptFriend((client_accept_friend_t*)request.getPayload(), data);
        break;
    case CLIENT_FRIEND_REQUEST:
        friendRequest((client_friend_request_t*)request.getPayload(), data);
        break;
    case CLIENT_BYE:
        bye((client_bye_t*)request.getPayload(), data);
        break;
    case CLIENT_CALL:
        call((client_call_t*)request.getPayload(), data);
        break;
    case CLIENT_GOODBYE:
        goodbye((client_goodbye_t*)request.getPayload(), data);
        break;
    case CLIENT_REGISTER:
        clientRegister((client_register_t*)request.getPayload(), data);
        break;
    default:
        throw "Unknown request.";
    }
}

void RequestHandler::ping(client_ping_t* payload, SharedData& data)
{
}

static void callbackWrite(const boost::system::error_code& ec)
{
    if (ec)
        std::cerr << ec.message() << std::endl;
}

void RequestHandler::hello(client_hello_t* payload, SharedData& data)
{
    std::cout << "Username: " << payload->username << std::endl;
    std::cout << "Password: " << payload->password << std::endl;

    boost::asio::async_write(
        data.socket,
        boost::asio::buffer(std::string("Salut maman, je passe allah tele!")),
        boost::bind(callbackWrite, boost::asio::placeholders::error));
}

void RequestHandler::friendRequest(client_friend_request_t* payload, SharedData& data)
{
}

void RequestHandler::goodbye(client_goodbye_t* payload, SharedData& data)
{
}

void RequestHandler::clientRegister(client_register_t* payload, SharedData& data)
{
}

void RequestHandler::call(client_call_t* payload, SharedData& data)
{
}

void RequestHandler::bye(client_bye_t*, SharedData& data)
{
}

void RequestHandler::acceptFriend(client_accept_friend_t* payload, SharedData& data)
{
}

void RequestHandler::friendStatus(client_friend_status_t*, SharedData& data)
{
}
