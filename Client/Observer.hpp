//
// Created by Quentin Liardeaux on 10/3/19.
//

#ifndef BABEL_SERVER_OBSERVER_HPP
#define BABEL_SERVER_OBSERVER_HPP

#include "Subject.hpp"

class Subject;

class Observer {
public:
    Observer(Subject *sub);

    virtual void update() = 0;
protected:
    const Subject *getSubject() const;
private:
    Subject *sub_;
};


#endif //BABEL_SERVER_OBSERVER_HPP
