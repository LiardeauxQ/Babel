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
    subjects_.push_back(boost::shared_ptr<Subject>(new Subject("loginResponse")));
    subjects_.push_back(boost::shared_ptr<Subject>(new Subject("loginResponse")));
    for (auto sub : subjects_)
        notifHandler_->registerEvent(sub);
    notifHandler_->attachToEvent(observer_, "close");
    notifHandler_->attachToEvent(observer_, "login");
    notifHandler_->attachToEvent(observer_, "register");
    notifHandler_->attachToEvent(observer_, "fetchFriends");
}

void AppManager::notifySubject(const std::string &label, std::map<std::string, void*> &userInfo)
{
    for (auto sub : subjects_) {
        if (sub->getLabel() == label) {
            sub->notify(userInfo);
            break;
        }
    }
}

void AppManager::notifyResponse(const std::string &label, RESULT result)
{
    std::map<std::string, void*> userInfo;

    std::cout << label << std::endl;
    userInfo["type"] = (void*)(std::string("result").c_str());
    userInfo["result"] = (void*)(&result);
    notifySubject("loginResponse", userInfo);
}

void AppManager::askToLog(const std::string &username, const std::string &password)
{
    std::map<std::string, void*> userInfo;

    std::cout << "Login:" << std::endl;
    std::cout << username << std::endl;
    userInfo["username"] = (void *)(username.c_str());
    userInfo["password"] = (void *)(password.c_str());

    void *payload = serverHandler_->send(CLIENT_HELLO, userInfo);
    int result = (*(server_hello_response_t*)payload).result;

    if (result == RESULT::OK)
        UserSession::get()->connectUser(username);
    notifyResponse("loginResponse", result == RESULT::OK ? RESULT::OK : RESULT::KO);
}

void AppManager::askToRegister(const std::string &username, const std::string &password)
{
    std::map<std::string, void*> userInfo;

    std::cout << "Register:" << std::endl;
    std::cout << username << std::endl;
    userInfo["username"] = (void *)(username.c_str());
    userInfo["password"] = (void *)(password.c_str());

    void *payload = serverHandler_->send(CLIENT_REGISTER, userInfo);
    int result = (*(server_register_response_t*)payload).result;

    if (result == RESULT::OK)
        UserSession::get()->connectUser(username);
    notifyResponse("registerResponse", result == RESULT::OK ? RESULT::OK : RESULT::KO);
}

void AppManager::askToFetchFriends()
{
    std::map<std::string, void*> userInfo;

    void *payload = serverHandler_->send(CLIENT_FRIEND_STATUS, userInfo);
    server_friend_status_t *srv = (server_friend_status_t*)payload;

    std::cout << "friends" << std::endl;
    for (int i = 0 ; i < MAX_FRIENDS ; i++) {
        std::cout << srv->usernames[i] << std::endl;
    }
}

void AppManager::close()
{
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
    if (!strcmp(typeValue, "fetchFriends"))
        manager_.askToFetchFriends();
}
