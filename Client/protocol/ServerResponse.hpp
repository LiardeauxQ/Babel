//
// Created by Quentin Liardeaux on 10/5/19.
//

#ifndef BABEL_SERVER_SERVERRESPONSE_HPP
#define BABEL_SERVER_SERVERRESPONSE_HPP

#include <QObject>
#include <iostream>
#include <queue>
#include <vector>
#include <tuple>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include "../NotificationHandler.hpp"
#include "Message.hpp"

class ServerResponse: public QObject {
    Q_OBJECT
public:
    ServerResponse(boost::shared_ptr<std::queue<Message>> responses,
            boost::shared_ptr<boost::mutex> mutex,
            boost::shared_ptr<NotificationHandler> notifHandler);
    ~ServerResponse() = default;
protected:
    bool event(QEvent *event);
private:

    void hello(void *payload);
    void registerResponse(void *payload);
    void fetchFriends(void *payload);
    void call(void *payload);
    void callResponse(void *payload);
    void acceptCall(void *payload);
    void acceptCallResponse(void *payload);
    void notifyObservers(const std::string &label, std::map<std::string, void*> userInfo);
    void notifyObservers(const std::string &label, const std::string &type, void* payload);
    void initHandler();
    void dispatchPayloads(Message message);

    boost::shared_ptr<std::queue<Message>> responses_;
    boost::shared_ptr<boost::mutex> mutex_;
    boost::shared_ptr<NotificationHandler> notifHandler_;
    std::vector<boost::shared_ptr<Subject>> subjects_;
};

#endif //BABEL_SERVER_SERVERRESPONSE_HPP
