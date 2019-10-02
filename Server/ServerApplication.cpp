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
    , sessions_()
{
    accept();
}

void ServerApplication::accept()
{
    auto session = Session::create(context_, database_, sessions_);

    acceptor_.async_accept(
        session->getSocket(),
        boost::bind(&ServerApplication::handleAccept,
            this,
            session,
            boost::asio::placeholders::error));
}

void ServerApplication::handleAccept(const boost::shared_ptr<Session>& session, const boost::system::error_code& ec)
{
    if (!ec)
        session->run();
    accept();
}