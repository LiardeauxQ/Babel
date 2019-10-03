//
// Created by Quentin Liardeaux on 10/3/19.
//

#include "NotificationHandler.hpp"

void NotificationHandler::registerEvent(Subject *sub)
{
    subjects_.push_back(sub);
    for (const auto& status : observers_) {
        if (status.label == sub->getLabel() && !status.isAttach) {
            sub->attach(status.obs);
            break;
        }
    }
}

void NotificationHandler::unregisterEvent(Subject *sub)
{
    size_t i = 0;

    sub->dettachAll();
    for (auto subject : subjects_) {
        if (subject == sub) {
            subjects_.erase(subjects_.begin() + i);
            break;
        }
        i++;
    }
}

void NotificationHandler::attachToEvent(Observer *obs, const std::string &label)
{
    ObserverStatus status = {obs, label, false};

    for (auto sub : subjects_) {
        if (sub->getLabel() == label) {
            sub->attach(obs);
            status.isAttach = true;
            break;
        }
    }
    observers_.push_back(status);
}

void NotificationHandler::dettachToEvent(Observer *obs, const std::string &label)
{
    size_t i = 0;

    for (auto sub :subjects_) {
        if (sub->getLabel() == label) {
            sub->dettach(obs);
            break;
        }
    }
    for (const auto &status : observers_) {
        if  (status.obs == obs) {
            observers_.erase(observers_.begin() + i);
            break;
        }
        i++;
    }
}