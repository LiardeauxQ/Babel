//
// Created by Quentin Liardeaux on 10/3/19.
//

#include "AppManager.hpp"

std::vector<std::tuple<std::string, AppManager::commandPtr>> AppManager::commands = {
        std::make_tuple("login", &AppManager::askToLog),
        std::make_tuple("register", &AppManager::askToRegister),
        std::make_tuple("goodbye", &AppManager::askToDisconnect),
        std::make_tuple("close", &AppManager::close),
        std::make_tuple("fetchFriends", &AppManager::askToFetchFriends),
        std::make_tuple("call", &AppManager::askToCall),
        std::make_tuple("endCall", &AppManager::askToEndCall),
        std::make_tuple("acceptCall", &AppManager::askToAcceptCall),
        std::make_tuple("callServer", &AppManager::receiveCallFromServer),
        std::make_tuple("callAcceptServer", &AppManager::receiveCallAcceptServer),
        std::make_tuple("callAcceptResponse", &AppManager::receiveCallAcceptResponse)
};

AppManager::AppManager(int argc, char *argv[]) :
        notifHandler_(boost::shared_ptr<NotificationHandler>(new NotificationHandler())),
        widget_(ui::BabelMainWindow(notifHandler_, nullptr))
{
    InputOptionsHandler handler(argc, argv);

    isUdpOnly_ = handler.isUdpOnly();
    if (!isUdpOnly_) {
        remoteIpAddress_ = handler.getRemoteIp();
        port_ = handler.getPort();
        localIpAddress_ = handler.getLocalIp();
        serverHandler_ = boost::shared_ptr<ServerHandler>(new ServerHandler(remoteIpAddress_, port_, notifHandler_));
        observer_ = boost::shared_ptr<AppManagerObserver>(new AppManagerObserver(this));
    }
}

void AppManager::start()
{
    if (isUdpOnly_) {
        runUdpServer(remoteIpAddress_, port_ + 1);
        return;
    }
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
    notifHandler_->attachToEvent(observer_, "endCall");
    notifHandler_->attachToEvent(observer_, "goodbye");
    notifHandler_->attachToEvent(observer_, "callAcceptServer");
    notifHandler_->attachToEvent(observer_, "callAcceptResponse");
}

void AppManager::askToLog(std::map<std::string, void*> &userInfo)
{
    std::string username((char*)userInfo.find("username")->second);
    std::string password((char*)userInfo.find("password")->second);
    std::map<std::string, void*> newUserInfo;

    std::cout << "Login:" << std::endl;
    std::cout << username << std::endl;
    newUserInfo["username"] = (void *)(username.c_str());
    newUserInfo["password"] = (void *)(password.c_str());

    serverHandler_->send(CLIENT_HELLO, newUserInfo);
}

void AppManager::askToRegister(std::map<std::string, void*> &userInfo)
{
    std::string username((char*)userInfo.find("username")->second);
    std::string password((char*)userInfo.find("password")->second);
    std::map<std::string, void*> newUserInfo;

    std::cout << "Register:" << std::endl;
    std::cout << username << std::endl;
    newUserInfo["username"] = (void *)(username.c_str());
    newUserInfo["password"] = (void *)(password.c_str());

    serverHandler_->send(CLIENT_REGISTER, newUserInfo);
}

void AppManager::askToFetchFriends(std::map<std::string, void*> &userInfo)
{
    serverHandler_->send(CLIENT_FRIEND_STATUS, std::map<std::string, void*>());
}

void AppManager::askToCall(std::map<std::string, void*> &userInfo)
{
    std::string username((char*)userInfo.find("username")->second);
    std::map<std::string, void*> newUserInfo;
    int port = serverHandler_->getPort() + 1;

    newUserInfo["username"] = (void*)(username.c_str());
    newUserInfo["addressIp"] = (void*)(localIpAddress_.c_str());
    newUserInfo["port"] = (void*)(&port);
    serverHandler_->send(CLIENT_CALL, newUserInfo);
}

void AppManager::askToAcceptCall(std::map<std::string, void*> &userInfo)
{
    std::string username((char*)userInfo.find("username")->second);
    std::map<std::string, void*> newUserInfo;
    int port = serverHandler_->getPort() + 1;

    newUserInfo["username"] = (void*)(username.c_str());
    newUserInfo["addressIp"] = (void*)(localIpAddress_.c_str());
    newUserInfo["port"] = (void*)(&port);
    serverHandler_->send(CLIENT_ACCEPT_CALL, newUserInfo);
}

void AppManager::askToEndCall(std::map<std::string, void*> &userInfo)
{
    serverHandler_->send(CLIENT_BYE, std::map<std::string, void*>());
}

void AppManager::askToDisconnect(std::map<std::string, void*> &userInfo)
{
    serverHandler_->send(CLIENT_GOODBYE, std::map<std::string, void*>());
}

void AppManager::addFriendInfo(std::string username, std::string ip, short port)
{
    friendsInfo_.push_back(FriendInfo{username, ip, port});
}

void AppManager::startSoundConnection(std::string username)
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
    runUdpServer(info.ipAddress, info.port);
}

void AppManager::runUdpServer(std::string ipAddress, short port)
{
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string(ipAddress), port);

    soundServerHandler_ = boost::shared_ptr<SoundServerHandler>(new SoundServerHandler(endpoint));
    soundServerHandler_->start();
}

void AppManager::close(std::map<std::string, void*> &userInfo)
{
    if (!isUdpOnly_) {
        askToDisconnect(userInfo);
        serverHandler_->send(-1, std::map<std::string, void *>());
        serverHandler_->stop();
    }
    if (soundServerHandler_.get())
        soundServerHandler_->stop();
}

void AppManager::call(std::map<std::string, void*> &userInfo)
{
}

void AppManager::requestFriends(std::map<std::string, void*> &userInfo)
{
}

void AppManager::receiveCallFromServer(std::map<std::string, void*> &userInfo)
{
    server_call_t *srv = (server_call_t*)userInfo.find("payload")->second;
    std::cout << "app manager and call server" << std::endl;
    addFriendInfo(srv->username, srv->ip, srv->port);
}

void AppManager::receiveCallAcceptServer(std::map<std::string, void*> &userInfo)
{
    server_accept_call_t *srv = (server_accept_call_t*)userInfo.find("payload")->second;
    std::cout << "app manager list call accept server" << std::endl;

    addFriendInfo(srv->username, srv->ip, srv->port);
    startSoundConnection(srv->username);
}

void AppManager::receiveCallAcceptResponse(std::map<std::string, void*> &userInfo)
{
    startSoundConnection(friendsInfo_.back().username);
}

AppManager::AppManagerObserver::AppManagerObserver(AppManager *manager) :
    Observer(nullptr),
    manager_(manager)
{
}

void AppManager::AppManagerObserver::update(std::map<std::string, void*> userInfo) {
    std::string typeValue((char *)(userInfo.find("type")->second));

    for (auto command : commands) {
        std::string type(std::get<0>(command).c_str());
        if ((type != "login" && type != "register") && !UserSession::get()->isConnected())
            continue;
        if (type == typeValue)
            boost::bind(std::get<1>(command), manager_, _1)(userInfo);
    }
}