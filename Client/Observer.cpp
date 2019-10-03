//
// Created by Quentin Liardeaux on 10/3/19.
//

#include "Observer.hpp"

Observer::Observer(Subject *sub) :
        sub_(sub)
{
    if (sub_)
        sub_->attach(this);
}