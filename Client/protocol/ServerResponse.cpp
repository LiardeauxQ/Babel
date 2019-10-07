//
// Created by Quentin Liardeaux on 10/5/19.
//

#include "ServerResponse.hpp"

#include <QThread>

ServerResponse::ServerResponse(boost::shared_ptr<std::queue<Message>> responses,
        boost::shared_ptr<boost::mutex> mutex,
        boost::shared_ptr<NotificationHandler> notifHandler) :
        QObject(nullptr),
        responses_(responses),
        mutex_(mutex),
        notifHandler_(notifHandler)
{}

bool ServerResponse::event(QEvent *event)
{
    mutex_->lock();
    Message message = responses_->front();
    std::cout << "receiving id " << message.getId() << std::endl;
    dispatchPayloads(message);
    responses_->pop();
    mutex_->unlock();
    return true;
}

void ServerResponse::dispatchPayloads(Message message)
{
    switch (message.getId()) {
        case SERVER_HELLO_RESPONSE:
            hello(message.getPayload());
            break;
        case SERVER_REGISTER_RESPONSE:
            registerResponse(message.getPayload());
            break;
        case SERVER_FRIEND_STATUS:
            fetchFriends(message.getPayload());
            break;
        case SERVER_CALL_RESPONSE:
            callResponse(message.getPayload());
            break;
        case SERVER_CALL:
            call(message.getPayload());
            break;
        case SERVER_ACCEPT_CALL:
            acceptCall(message.getPayload());
            break;
        case SERVER_ACCEPT_CALL_RESPONSE:
            acceptCallResponse(message.getPayload());
            break;
        default:
            break;
    }
}

void ServerResponse::notifyObservers(const std::string &label, std::map<std::string, void *> userInfo)
{
    for (auto sub : subjects_) {
        if (sub->getLabel() == label) {
            sub->notify(userInfo);
            break;
        }
    }
}

void ServerResponse::notifyObservers(const std::string &label, const std::string &type, void *payload)
{
    std::map<std::string, void*> userInfo;

    userInfo["type"] = strdup(type.c_str());
    userInfo["payload"] = payload;
    notifyObservers(label, userInfo);
}

void ServerResponse::hello(void *payload)
{
    static bool isInit = false;

    if (!isInit) {
        subjects_.push_back(boost::shared_ptr<Subject>(new Subject("loginResponse")));
        notifHandler_->registerEvent(subjects_.back());
        isInit = true;
    }
    server_hello_response_t *srv = (server_hello_response_t*)payload;
    std::map<std::string, void*> userInfo;

    userInfo["type"] = strdup("hello");
    userInfo["result"] = (void*)(&srv->result);
    notifyObservers("loginResponse", userInfo);
}

void ServerResponse::registerResponse(void *payload)
{
    static bool isInit = false;

    if (!isInit) {
        subjects_.push_back(boost::shared_ptr<Subject>(new Subject("registerResponse")));
        notifHandler_->registerEvent(subjects_.back());
        isInit = true;
    }
    server_register_response_t *srv = (server_register_response_t*)payload;
    std::map<std::string, void*> userInfo;

    userInfo["type"] = strdup("register");
    userInfo["result"] = (void*)(&srv->result);
    std::cout << "Receive register resposne" << srv->result << std::endl;
    notifyObservers("registerResponse", userInfo);
}

void ServerResponse::fetchFriends(void *payload)
{
    static bool isInit = false;

    if (!isInit) {
        subjects_.push_back(boost::shared_ptr<Subject>(new Subject("fetchFriendsResponse")));
        notifHandler_->registerEvent(subjects_.back());
        isInit = true;
    }
    notifyObservers("fetchFriendsResponse", "fetchFriends", payload);
}

void ServerResponse::callResponse(void *payload)
{
    static bool isInit = false;

    if (!isInit) {
        subjects_.push_back(boost::shared_ptr<Subject>(new Subject("callResponse")));
        notifHandler_->registerEvent(subjects_.back());
        isInit = true;
    }
    notifyObservers("callResponse", "callResponse", payload);
}

void ServerResponse::call(void *payload)
{
    static bool isInit = false;

    if (!isInit) {
        subjects_.push_back(boost::shared_ptr<Subject>(new Subject("callServer")));
        notifHandler_->registerEvent(subjects_.back());
        isInit = true;
    }
    std::cout << "call server "<< std::endl;
    notifyObservers("callServer", "callServer", payload);
}

void ServerResponse::acceptCallResponse(void *payload)
{
    static bool isInit = false;

    if (!isInit) {
        subjects_.push_back(boost::shared_ptr<Subject>(new Subject("callAcceptResponse")));
        notifHandler_->registerEvent(subjects_.back());
        isInit = true;
    }
    std::cout << "call accept response" << std::endl;
    notifyObservers("callAcceptResponse", "callAcceptResponse", payload);
}

void ServerResponse::acceptCall(void *payload)
{
    static bool isInit = false;

    if (!isInit) {
        subjects_.push_back(boost::shared_ptr<Subject>(new Subject("callAcceptServer")));
        notifHandler_->registerEvent(subjects_.back());
        isInit = true;
    }
    std::cout << "call accept server "<< std::endl;
    notifyObservers("callAcceptServer", "callAcceptServer", payload);
}