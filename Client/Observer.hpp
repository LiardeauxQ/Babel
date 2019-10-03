//
// Created by Quentin Liardeaux on 10/3/19.
//

#ifndef BABEL_SERVER_OBSERVER_HPP
#define BABEL_SERVER_OBSERVER_HPP

#include "Subject.hpp"
#include <iostream>

class Subject;

class Observer {
public:
    Observer(Subject *sub = nullptr);

    virtual void update(std::map<std::string, void*>) = 0;
protected:
    const Subject *getSubject() const { return sub_; }
private:
    Subject *sub_;
};

#endif //BABEL_SERVER_OBSERVER_HPP
