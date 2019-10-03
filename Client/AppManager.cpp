//
// Created by Quentin Liardeaux on 10/3/19.
//

#include "AppManager.hpp"

AppManager::AppManager(std::unique_ptr<ServerRequest> request) :
    request_(std::move(request)),
    session_(UserSession()),
    notifHandler_(QSharedPointer<NotificationHandler>(new NotificationHandler())),
    widget_(ui::MainWidget(nullptr, notifHandler_)),
    observer_(AppManagerObserver(this))
{
    notifHandler_->attachToEvent(&observer_, "login");
    notifHandler_->attachToEvent(&observer_, "register");
}

void AppManager::start()
{
    request_->start();
    widget_.resize(500, 500);
    widget_.show();
}

void AppManager::askToLog(const std::string &username, const std::string &password)
{
    std::cout << "Login:" << std::endl;
    std::cout << username << std::endl;
    request_->sendHello(username, password);
}

void AppManager::askToRegister(const std::string &username, const std::string &password)
{
    std::cout << "Register:" << std::endl;
    std::cout << username << std::endl;
    request_->sendRegister(username, password);
}

void AppManager::call()
{
}

void AppManager::requestFriends()
{
}

AppManager::AppManagerObserver::AppManagerObserver(AppManager *manager) :
    Observer(nullptr),
    manager_(manager)
{
}

void AppManager::AppManagerObserver::update(std::map<std::string, void*> userInfo)
{
    auto typeValue = (char*)(userInfo.find("type")->second);

    if (!strcmp(typeValue, "login") || !strcmp(typeValue, "register")) {
        auto usernameValue = (char*)(userInfo.find("username")->second);
        auto passwordValue = (char*)(userInfo.find("password")->second);

        if (!strcmp(typeValue, "login"))
            manager_->askToLog(usernameValue, passwordValue);
        else if (!strcmp(typeValue, "register"))
            manager_->askToRegister(usernameValue, passwordValue);
    }
}
