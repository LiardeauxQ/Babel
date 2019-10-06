//
// Created by Quentin Liardeaux on 10/4/19.
//

#include "ServerCommunication.hpp"

#include <QApplication>

ServerCommunication::ServerCommunication(const std::string &ipAddress, int port) :
        context_()
        , socket_(BoostTcp::socket(context_))
        , ipAddress_(ipAddress)
        , port_(port)
{
}

ServerCommunication::~ServerCommunication()
{
}

void ServerCommunication::stop()
{
    socket_.close();
}

void ServerCommunication::start()
{
    socket_.connect(BoostTcp::endpoint(
            boost::asio::ip::address::from_string(ipAddress_), port_));
}

void ServerCommunication::sendRequest(boost::shared_ptr<boost::mutex> mutex, boost::shared_ptr<std::queue<Message>> queue)
{
    Message message;

    while (true) {
        mutex->lock();
        if (queue->size() <= 0) {
            mutex->unlock();
            continue;
        }
        if (queue->front().getId() == -1) {
            mutex->unlock();
            break;
        }
        write(queue->front());
        queue->pop();
        mutex->unlock();
    }
}

void ServerCommunication::write(Message &message)
{
    client_register_t *clt = (client_register_t*)(message.getPayload());

    socket_.send(boost::asio::buffer(message.getHeaderRaw(), HEADER_SIZE));
    socket_.send(boost::asio::buffer(message.getPayload(), message.getPayloadSize()));
}

void ServerCommunication::receiveResponse(ServerResponse *response, boost::shared_ptr<boost::mutex> mutex, boost::shared_ptr<std::queue<Message>> queue)
{
    Message message = read();

    if (message.getId() <= 0)
        return;
    mutex->lock();
    queue->push(message);
    QCoreApplication::postEvent(response, new QEvent(QEvent::User));
    mutex->unlock();
    if (socket_.is_open())
        receiveResponse(response, mutex, queue);
}

Message ServerCommunication::read()
{
    Message message;

    try {
        socket_.receive(boost::asio::buffer(message.getHeaderRaw(), HEADER_SIZE));
        if (message.getId() >= 0) {
            message.setupPayload();
            socket_.receive(boost::asio::buffer(message.getPayload(), message.getPayloadSize()));
        }
        std::cout << "reading  id " << message.getId() << std::endl;
    } catch (std::exception e) {
        std::cout << "Connection has been closed " << std::endl;
    }
    return message;
}