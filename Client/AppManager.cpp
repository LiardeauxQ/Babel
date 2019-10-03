//
// Created by Quentin Liardeaux on 10/3/19.
//

#include "AppManager.hpp"

AppManager::AppManager(std::unique_ptr<ServerRequest> request) :
    request_(std::move(request)),
    session_(UserSession()),
    notifHandler_(QSharedPointer<NotificationHandler>(new NotificationHandler())),
    widget_(ui::MainWidget(nullptr, notifHandler_))
{
}

void AppManager::start()
{
    widget_.resize(500, 500);
    widget_.show();
}

void AppManager::askToLog()
{
}

void AppManager::askToRegister()
{
}

void AppManager::call()
{
}

void AppManager::requestFriends()
{
}

AppManager::AppManagerObservable::AppManagerObservable(AppManager *manager) :
    Observer(nullptr),
    manager_(manager)
{
}

void AppManager::AppManagerObservable::update()
{

}