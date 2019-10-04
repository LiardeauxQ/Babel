//
// Created by Quentin Liardeaux on 10/3/19.
//

#include "Subject.hpp"

Subject::Subject(const std::string &label) :
    label_(label)
{}

Subject::~Subject()
{
    observables_.empty();
}

void Subject::attach(boost::shared_ptr<Observer> obs)
{
    observables_.push_back(obs);
}

void Subject::dettach(boost::shared_ptr<Observer> obs)
{
    size_t i = 0;

    for (auto observer : observables_) {
        if (observer.get() == obs.get()) {
            observables_.erase(observables_.begin() + i);
            return;
        }
        i++;
    }
}

void Subject::dettachAll()
{
    observables_.empty();
}

void Subject::notify(std::map<std::string, void*> userInfo)
{
    for (auto obs : observables_) {
        obs->update(userInfo);
    }
}
