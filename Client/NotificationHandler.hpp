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
    void registerEvent(boost::shared_ptr<Subject> sub);
    void unregisterEvent(boost::shared_ptr<Subject> sub);

    void attachToEvent(boost::shared_ptr<Observer> obs, const std::string &label);
    void dettachToEvent(boost::shared_ptr<Observer> obs, const std::string &label);
private:
    struct ObserverStatus {
        boost::shared_ptr<Observer> obs;
        std::string label;
        bool isAttach;
    };

    std::vector<ObserverStatus> observers_;
    std::vector<boost::shared_ptr<Subject>> subjects_;
};

#endif //BABEL_SERVER_NOTIFICATIONHANDLER_HPP
