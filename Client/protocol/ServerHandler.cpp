//
// Created by Quentin Liardeaux on 10/4/19.
//

#include "ServerHandler.hpp"

ServerHandler::ServerHandler(const std::string &ipAddress, int port, boost::shared_ptr<NotificationHandler> notifHandler) :
    notifHandler_(notifHandler),
    communicationHandler_(ServerCommunication(ipAddress, port))
{
    requestMutex_ = boost::shared_ptr<boost::mutex>(new boost::mutex());
    requests_ = boost::shared_ptr<std::queue<Message>>(new std::queue<Message>());
    responseMutex_ = boost::shared_ptr<boost::mutex>(new boost::mutex());
    responses_ = boost::shared_ptr<std::queue<Message>>(new std::queue<Message>());
    requestThread_ = boost::thread(&ServerCommunication::sendRequest, &communicationHandler_, requestMutex_, requests_);
}

ServerHandler::~ServerHandler()
{
    requestThread_.join();
}

void ServerHandler::start()
{
    communicationHandler_.start();
}

RESULT ServerHandler::send(int id, std::map<std::string, void *> userInfo)
{
    requestMutex_->lock();
    if (id != -1)
        requests_->push(ServerRequest::createRequest(id, userInfo));
    else
        requests_->push(Message(-1, 0, nullptr));
    requestMutex_->unlock();
    if (id != -1)
        return receive();
    return RESULT::KO;
}

RESULT ServerHandler::receive() {
    responseThread_ = boost::thread(&ServerCommunication::receiveResponse, &communicationHandler_, responseMutex_,
                                    responses_);
    responseThread_.join();
    if (responses_->size() > 0) {
        RESULT result = ServerResponse::receiveResponse(responses_->front());

        responses_->pop();
        return result;
    } else
        std::cout << "no responses" << std::endl;
    return RESULT::KO;
}