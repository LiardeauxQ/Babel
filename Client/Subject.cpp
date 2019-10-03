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

void Subject::attach(Observer *obs)
{
    observables_.push_back(obs);
}

void Subject::dettach(Observer *obs)
{
    size_t i = 0;

    for (auto observer : observables_) {
        if (observer == obs) {
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

void Subject::notify()
{
    for (auto obs : observables_) {
        std::cout << "notify" << std::endl;
        obs->update();
    }
}
