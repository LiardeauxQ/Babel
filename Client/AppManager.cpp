//
// Created by Quentin Liardeaux on 10/3/19.
//

#include "AppManager.hpp"

AppManager::AppManager(int argc, char *argv[]) :
        notifHandler_(boost::shared_ptr<NotificationHandler>(new NotificationHandler())),
        widget_(ui::BabelMainWindow(notifHandler_, nullptr))
{
    if (argc < 4)
        throw "Invalid arguments";
    remoteIpAddress_ = argv[1];
    port_ = atoi(argv[2]);
    localIpAddress_ = argv[3];
    serverHandler_ = boost::shared_ptr<ServerHandler>(new ServerHandler(remoteIpAddress_, port_, notifHandler_));
    observer_ = boost::shared_ptr<AppManagerObserver>(new AppManagerObserver(*this));
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
    notifHandler_->attachToEvent(observer_, "acceptCall");
    notifHandler_->attachToEvent(observer_, "callServer");
    notifHandler_->attachToEvent(observer_, "callAcceptServer");
    notifHandler_->attachToEvent(observer_, "callAcceptResponse");
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

    userInfo["username"] = (void*)(username.c_str());
    userInfo["addressIp"] = (void*)(localIpAddress_.c_str());
    userInfo["port"] = (void*)(&port);
    serverHandler_->send(CLIENT_CALL, userInfo);
}

void AppManager::askToAcceptCall(const std::string &username)
{
    std::map<std::string, void*> userInfo;
    int port = serverHandler_->getPort() + 1;

    userInfo["username"] = (void*)(username.c_str());
    userInfo["addressIp"] = (void*)(localIpAddress_.c_str());
    userInfo["port"] = (void*)(&port);
    serverHandler_->send(CLIENT_ACCEPT_CALL, userInfo);
}

void AppManager::addFriendInfo(const std::string &username,
        const std::string &ipAddress, short port)
{
    friendsInfo_.push_back(FriendInfo{username, ipAddress, port});
}

void AppManager::startSoundUdpServer(const std::string &username)
{
    FriendInfo info;

    for (auto friendInfo : friendsInfo_) {
        if (friendInfo.username == username) {
            info = friendInfo;
            break;
        }
    }
    std::cout << "start connection with " << info.ipAddress << ":" << info.port << std::endl;
    if (info.port == 0)
        return;
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string(info.ipAddress), info.port);

    soundServerHandler_ = boost::shared_ptr<SoundServerHandler>(new SoundServerHandler(endpoint));
    soundServerHandler_->start();
}

void AppManager::close()
{
    serverHandler_->send(-1, std::map<std::string, void*>());
    serverHandler_->stop();
    if (soundServerHandler_.get())
        soundServerHandler_->stop();
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
    auto typeValue = (char *)(userInfo.find("type")->second);

    if (!strcmp(typeValue, "login") || !strcmp(typeValue, "register")) {
        auto usernameValue = (char *)(userInfo.find("username")->second);
        auto passwordValue = (char *)(userInfo.find("password")->second);

        if (!strcmp(typeValue, "login"))
            manager_.askToLog(usernameValue, passwordValue);
        if (!strcmp(typeValue, "register"))
            manager_.askToRegister(usernameValue, passwordValue);
    }
    if (!strcmp(typeValue, "close"))
        manager_.close();
    if (!UserSession::get()->isConnected())
        return;
    if (!strcmp(typeValue, "fetchFriends"))
        manager_.askToFetchFriends();
    if (!strcmp(typeValue, "call"))
        manager_.askToCall((char*)userInfo.find("username")->second);
    if (!strcmp(typeValue, "acceptCall"))
        manager_.askToAcceptCall((char*)userInfo.find("username")->second);
    if (!strcmp(typeValue, "callServer")) {
        server_call_t *srv = (server_call_t*)userInfo.find("payload")->second;

        std::cout << "app manager and call server" << std::endl;
        manager_.addFriendInfo(srv->username, srv->ip, srv->port);
    }
    if (!strcmp(typeValue, "callAcceptServer")) {
        server_accept_call_t *srv = (server_accept_call_t*)userInfo.find("payload")->second;
        std::cout << "app manager list call accept server" << std::endl;

        manager_.addFriendInfo(srv->username, srv->ip, srv->port);
        manager_.startSoundUdpServer(srv->username);
    }
    if (!strcmp(typeValue, "callAcceptResponse")) {
        manager_.startSoundUdpServer(manager_.friendsInfo_.back().username);
    }
}
