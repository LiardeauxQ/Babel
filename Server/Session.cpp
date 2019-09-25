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
    , request_()
{
}

void Session::run()
{
    boost::asio::async_read(
        socket_,
        boost::asio::buffer(request_.header(), REQUEST_HEADER_SIZE),
        boost::bind(&Session::receivePacket, this, boost::asio::placeholders::error));
}

void Session::receivePacket(const boost::system::error_code& ec)
{
    if (!ec) {
        std::cout << "Packet received" << std::endl;
        int size = request_.setupPayload();
        std::cout << "The packet is of size: " << size << std::endl;

        boost::asio::async_read(
            socket_,
            boost::asio::buffer(request_.payload(), size),
            boost::bind(&Session::receiveBody, this, boost::asio::placeholders::error));
    }
}

void Session::receiveBody(const boost::system::error_code& ec)
{
    if (!ec) {
        std::cout << "Body received" << std::endl;
        request_.handle();
    }
}
