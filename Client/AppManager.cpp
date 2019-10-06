//
// Created by Quentin Liardeaux on 10/3/19.
//

#include "AppManager.hpp"

AppManager::AppManager(boost::shared_ptr<ServerHandler> serverHandler,
        boost::shared_ptr<NotificationHandler> notifHandler) :
    notifHandler_(notifHandler),
    serverHandler_(serverHandler),
    widget_(ui::BabelMainWindow(notifHandler_, nullptr)),
    observer_(new AppManagerObserver(*this))
    {
}


void AppManager::start()
{
    initNotifications();
    widget_.show();
    serverHandler_->start();
}

void AppManager::initNotifications()
{
    notifHandler_->attachToEvent(observer_, "close");
    notifHandler_->attachToEvent(observer_, "login");
    notifHandler_->attachToEvent(observer_, "register");
    notifHandler_->attachToEvent(observer_, "fetchFriends");
    notifHandler_->attachToEvent(observer_, "call");
}

void AppManager::askToLog(const std::string &username, const std::string &password)
{
    std::map<std::string, void*> userInfo;

    std::cout << "Login:" << std::endl;
    std::cout << username << std::endl;
    userInfo["username"] = (void *)(username.c_str());
    userInfo["password"] = (void *)(password.c_str());

    serverHandler_->send(CLIENT_HELLO, userInfo);
}

void AppManager::askToRegister(const std::string &username, const std::string &password)
{
    std::map<std::string, void*> userInfo;

    std::cout << "Register:" << std::endl;
    std::cout << username << std::endl;
    userInfo["username"] = (void *)(username.c_str());
    userInfo["password"] = (void *)(password.c_str());

    serverHandler_->send(CLIENT_REGISTER, userInfo);
}

void AppManager::askToFetchFriends()
{
    std::map<std::string, void*> userInfo;

    serverHandler_->send(CLIENT_FRIEND_STATUS, userInfo);
}

void AppManager::askToCall(const std::string &username)
{
    std::map<std::string, void*> userInfo;
    int port = serverHandler_->getPort() + 1;

    std::cout << "to call:" << username << ":" <<std::endl;
    userInfo["username"] = (void*)(username.c_str());
    userInfo["addressIp"] = (void*)(serverHandler_->getIpAddress().c_str());
    userInfo["port"] = (void*)(&port);
    serverHandler_->send(CLIENT_CALL, userInfo);
}

void AppManager::askToAcceptCall(const std::string &username)
{
    std::map<std::string, void*> userInfo;
    int port = serverHandler_->getPort() + 1;

    userInfo["username"] = (void*)(username.c_str());
    userInfo["addressIp"] = (void*)(serverHandler_->getIpAddress().c_str());
    userInfo["port"] = (void*)(&port);
    serverHandler_->send(CLIENT_ACCEPT_CALL, userInfo);
}

void AppManager::close()
{
    serverHandler_->send(-1, std::map<std::string, void*>());
    serverHandler_->stop();
}

void AppManager::call()
{
}

void AppManager::requestFriends()
{
}

AppManager::AppManagerObserver::AppManagerObserver(AppManager &manager) :
    Observer(nullptr),
    manager_(manager)
{
}

void AppManager::AppManagerObserver::update(std::map<std::string, void*> userInfo) {
    auto typeValue = (char *) (userInfo.find("type")->second);

    if (!strcmp(typeValue, "login") || !strcmp(typeValue, "register")) {
        auto usernameValue = (char *) (userInfo.find("username")->second);
        auto passwordValue = (char *) (userInfo.find("password")->second);

        if (!strcmp(typeValue, "login"))
            manager_.askToLog(usernameValue, passwordValue);
        if (!strcmp(typeValue, "register"))
            manager_.askToRegister(usernameValue, passwordValue);
    }
    if (!strcmp(typeValue, "close"))
        manager_.close();
    if (!strcmp(typeValue, "fetchFriends"))
        manager_.askToFetchFriends();
    if (!strcmp(typeValue, "call"))
        manager_.askToCall((char*)userInfo.find("username")->second);
    if (!strcmp(typeValue, "acceptCall"))
        manager_.askToAcceptCall((char*)userInfo.find("username")->second);
}
