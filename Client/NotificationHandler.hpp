//
// Created by Quentin Liardeaux on 10/3/19.
//

#ifndef BABEL_SERVER_NOTIFICATIONHANDLER_HPP
#define BABEL_SERVER_NOTIFICATIONHANDLER_HPP

#include <vector>

#include "Observer.hpp"
#include "Subject.hpp"

class NotificationHandler {
public:
    void registerEvent(Subject *sub);
    void unregisterEvent(Subject *sub);

    void attachToEvent(Observer *obs, const std::string &label);
    void dettachToEvent(Observer *obs, const std::string &label);
private:
    struct ObserverStatus {
        Observer *obs;
        std::string label;
        bool isAttach;
    };

    std::vector<ObserverStatus> observers_;
    std::vector<Subject *> subjects_;
};

#endif //BABEL_SERVER_NOTIFICATIONHANDLER_HPP
