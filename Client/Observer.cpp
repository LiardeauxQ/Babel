//
// Created by Quentin Liardeaux on 10/3/19.
//

#include "Observer.hpp"

Observer::Observer(boost::shared_ptr<Subject> sub) :
        sub_(sub)
{
    if (sub_)
        sub_->attach(boost::shared_ptr<Observer>(this));
}