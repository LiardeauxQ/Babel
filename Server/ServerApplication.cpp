//
// Created by alex on 9/23/19.
//

#include "ServerApplication.hpp"

void ServerApplication::run()
{
    context_.run();
}

ServerApplication::ServerApplication(const ServerConfig& config)
    : database_(config.databaseUrl)
    , running_(false)
    , context_()
    , acceptor_(context_, BoostTcp::endpoint(BoostTcp::v4(), config.port))
    , clients_()
{
    accept();
}

void ServerApplication::accept()
{
    auto session = Session::create(context_);

    acceptor_.async_accept(
        session->getSocket(),
        boost::bind(&ServerApplication::handleAccept,
            this,
            session,
            boost::asio::placeholders::error));
}

void ServerApplication::handleAccept(std::shared_ptr<Session> session, const boost::system::error_code& ec)
{
    const std::string payload("Salut!");

    if (!ec)
        session->run();

    accept();
}

void ServerApplication::handleWrite(const boost::system::error_code&, size_t)
{
}
