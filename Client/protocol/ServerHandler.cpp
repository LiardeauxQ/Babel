//
// Created by Quentin Liardeaux on 10/4/19.
//

#include "ServerHandler.hpp"

ServerHandler::ServerHandler(const std::string &ipAddress, int port, boost::shared_ptr<NotificationHandler> notifHandler) :
    notifHandler_(notifHandler),
    communicationHandler_(ServerCommunication(ipAddress, port)),
    requestsMutex_(boost::shared_ptr<boost::mutex>(new boost::mutex())),
    requests_(boost::shared_ptr<std::queue<Message>>(new std::queue<Message>())),
    responsesMutex_(boost::shared_ptr<boost::mutex>(new boost::mutex())),
    responses_(boost::shared_ptr<std::queue<Message>>(new std::queue<Message>())),
    responsesHandler_(new ServerResponse(responses_, responsesMutex_, notifHandler_)),
    ipAddress_(ipAddress),
    port_(port)
{
}

ServerHandler::~ServerHandler()
{
    delete responsesHandler_;
}

void ServerHandler::start()
{
    communicationHandler_.start();
    requestsThread_ = boost::thread(&ServerCommunication::sendRequest, &communicationHandler_, requestsMutex_, requests_);
    responsesThread_ = boost::thread(&ServerCommunication::receiveResponse, &communicationHandler_, responsesHandler_, responsesMutex_, responses_);
}

void ServerHandler::stop()
{
    communicationHandler_.stop();
    requestsThread_.join();
    responsesThread_.join();
}

void ServerHandler::send(int id, std::map<std::string, void *> userInfo)
{
    requestsMutex_->lock();
    if (id != -1) {
        requests_->push(ServerRequest::createRequest(id, userInfo));
    } else
        requests_->push(Message(-1, 0, nullptr));
    requestsMutex_->unlock();
}

