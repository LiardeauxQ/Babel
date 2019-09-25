//
// Created by alex on 9/24/19.
//

#include "Session.hpp"

boost::shared_ptr<Session> Session::create(boost::asio::io_context& context)
{
    return boost::shared_ptr<Session>(new Session(context));
}

Session::Session(boost::asio::io_context& context)
    : socket_(context)
    , request_()
{
}

void Session::run(std::vector<boost::shared_ptr<Session>>& sessions)
{
    sessions.push_back(shared_from_this());

    boost::asio::async_read(
        socket_,
        boost::asio::buffer(data_.raw, REQUEST_HEADER_SIZE),
        boost::bind(&Session::receivePacket, shared_from_this(), boost::asio::placeholders::error));
}

void Session::receivePacket(const boost::system::error_code& ec)
{
    if (!ec) {
        data_.req.payload = malloc(data_.req.request_len);
        std::cout << "Packet received: " << data_.req.id << "." << std::endl;

        boost::asio::async_read(
            socket_,
            boost::asio::buffer(data_.req.payload, data_.req.request_len),
            boost::bind(&Session::receiveBody, shared_from_this(), boost::asio::placeholders::error));
    } else {
        std::cerr << "Error while receiving packet." << ec.message() << std::endl;
    }
}

void Session::receiveBody(const boost::system::error_code& ec)
{
    if (!ec) {
        std::cout << "Body received" << std::endl;
        RequestHandler::handleRequest(request_);
    } else {
        std::cerr << "Error while receiving body." << ec.message() << std::endl;
    }
}
