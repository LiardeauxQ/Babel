//
// Created by Quentin Liardeaux on 10/3/19.
//

#include "AppManager.hpp"

AppManager::AppManager(boost::shared_ptr<ServerHandler> serverHandler,
        boost::shared_ptr<NotificationHandler> notifHandler) :
    notifHandler_(notifHandler),
    serverHandler_(serverHandler),
    session_(UserSession()),
    widget_(ui::BabelMainWindow(notifHandler_, nullptr)),
    observer_(new AppManagerObserver(*this))
{
    notifHandler_->attachToEvent(observer_, "close");
    notifHandler_->attachToEvent(observer_, "login");
    notifHandler_->attachToEvent(observer_, "register");
}

void AppManager::start()
{
    //widget_.resize(500, 500);
    widget_.show();
    serverHandler_->start();
}

void AppManager::askToLog(const std::string &username, const std::string &password)
{
    std::map<std::string, void*> userInfo;

    std::cout << "Login:" << std::endl;
    std::cout << username << std::endl;
    userInfo["username"] = (void *)(username.c_str());
    userInfo["password"] = (void *)(password.c_str());
    serverHandler_->send(CLIENT_REGISTER, userInfo);
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

void AppManager::close()
{
    std::cout << "close" << std::endl;
    serverHandler_->send(-1, std::map<std::string, void*>());
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
}
