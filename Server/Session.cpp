//
// Created by alex on 9/24/19.
//

#include "Session.hpp"

boost::shared_ptr<Session> Session::create(boost::asio::io_context& context, Database& conn)
{
    return boost::shared_ptr<Session>(new Session(context, conn));
}

Session::Session(boost::asio::io_context& context, Database& conn)
    : socket_(context)
    , request_()
    , data_{conn, socket_}
{
}

void Session::run(std::vector<boost::shared_ptr<Session>>& sessions)
{
    sessions.push_back(shared_from_this());

    boost::asio::async_read(
        socket_,
        boost::asio::buffer(request_.getHeaderRaw(), HEADER_SIZE),
        boost::bind(&Session::receivePacket, shared_from_this(), boost::asio::placeholders::error));
}

void Session::receivePacket(const boost::system::error_code& ec)
{
    if (!ec) {
        request_.setupPayload();

        std::cout << "Id: " << request_.getId() << std::endl;
        std::cout << "Size payload: " << request_.getPayloadSize() << std::endl;

        boost::asio::async_read(
            socket_,
            boost::asio::buffer(request_.getPayload(), request_.getPayloadSize()),
            boost::bind(&Session::receiveBody, shared_from_this(), boost::asio::placeholders::error));
    } else {
        std::cerr << "Error while receiving packet." << ec.message() << std::endl;
    }
}

void Session::receiveBody(const boost::system::error_code& ec)
{
    if (!ec) {
        std::cout << "Body received" << std::endl;
        RequestHandler::handleRequest(request_, data_);
    } else {
        std::cerr << "Error while receiving body." << ec.message() << std::endl;
    }
}