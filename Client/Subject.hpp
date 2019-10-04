//
// Created by Quentin Liardeaux on 10/3/19.
//

#ifndef BABEL_SERVER_SUBJECT_HPP
#define BABEL_SERVER_SUBJECT_HPP

#include <vector>
#include <map>
#include <string>
#include <boost/shared_ptr.hpp>

#include "Observer.hpp"

class Observer;

class Subject {
public:
    Subject(const std::string &label);
    ~Subject();

    void attach(boost::shared_ptr<Observer> obs);
    void dettach(boost::shared_ptr<Observer> obs);
    void dettachAll();

    const std::string &getLabel() const { return label_; }
    void notify(std::map<std::string, void*> userInfo);
private:
    std::string label_;
    std::vector<boost::shared_ptr<Observer>> observables_;
};

#endif //BABEL_SERVER_SUBJECT_HPP
