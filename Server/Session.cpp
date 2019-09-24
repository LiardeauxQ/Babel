//
// Created by alex on 9/24/19.
//

#include "Session.hpp"

std::shared_ptr<Session> Session::create(boost::asio::io_context& context)
{
    return std::shared_ptr<Session>(new Session(context));
}

Session::Session(boost::asio::io_context& context)
    : socket_(context)
{
}

void Session::run()
{
    std::cout << "Running the connexion." << std::endl;
}
