//
// Created by Quentin Liardeaux on 10/3/19.
//

#include "Observer.hpp"

Observer::Observer(Subject *sub) :
    sub_(sub)
{
    sub_->attach(this);
}

const Subject *Observer::getSubject() const
{
    return sub_;
}