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

    try {
        boost::asio::write(socket_, boost::asio::buffer(message.getHeaderRaw(), HEADER_SIZE));
        boost::asio::write(socket_, boost::asio::buffer(message.getPayload(), message.getPayloadSize()));
    } catch (boost::exception &e) {
        std::cout << "Connection has been closed !" << std::endl;
    }
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
        boost::asio::read(socket_, boost::asio::buffer(message.getHeaderRaw(), HEADER_SIZE));
        std::cout << "Header " << message.getId() << " paylaod size " << message.getPayloadSize() << std::endl;
        if (message.getId() >= 0) {
            for (int i = 0 ; i < HEADER_SIZE ; i++) {
                printf("%d ", ((unsigned char*)(message.getHeaderRaw()))[i]);
            }
            printf("\n");
            message.setupPayload();
            boost::asio::read(socket_, boost::asio::buffer(message.getPayload(), message.getPayloadSize()));

        }
    } catch (std::exception e) {
        std::cout << "Connection has been close !" << std::endl;
    }
    return message;
}