//
// Created by alex on 9/23/19.
//

#include "ServerApplication.hpp"

void ServerApplication::run()
{
    context_.run();
    running_ = true;

    doAccept();
    char data[1024];
    boost::system::error_code ec;

    while (running_) {
        for (auto &c: clients_) {
            std::cout << "Reading client: " << c.release() << std::endl;
            size_t len = c.read_some(boost::asio::buffer(data), ec);

            if (ec == boost::asio::error::eof)
                c.close();

            for (int i = 0; i < len; ++i) {
                std::cout << data[i];
            }

            std::cout << std::endl;
        }
    }
}

void ServerApplication::doAccept()
{
    acceptor_.async_accept([this](boost::system::error_code ec, BoostTcp::socket socket) {
        std::cout << "Client accepted." << std::endl;
        if (!ec)
            clients_.push_back(std::move(socket));

        doAccept();
    });
}

ServerApplication::ServerApplication(const ServerConfig& config)
    : database_(config.databaseUrl)
    , running_(false)
    , context_()
    , acceptor_(context_, BoostTcp::endpoint(BoostTcp::v4(), config.port))
    , clients_()
{
}
