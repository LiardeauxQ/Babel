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
    Observer(boost::shared_ptr<Subject> sub = nullptr);

    virtual void update(std::map<std::string, void*> userInfo) = 0;
protected:
    const boost::shared_ptr<Subject> getSubject() const { return sub_; }
private:
    boost::shared_ptr<Subject> sub_;
};

#endif //BABEL_SERVER_OBSERVER_HPP
