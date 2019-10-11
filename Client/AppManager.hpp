//
// Created by Quentin Liardeaux on 10/3/19.
//

#ifndef BABEL_SERVER_APPMANAGER_HPP
#define BABEL_SERVER_APPMANAGER_HPP

#include <memory>
#include <QSharedPointer>
#include <tuple>

#include "protocol/ServerHandler.hpp"
#include "protocol/SoundServerHandler.hpp"
#include "protocol/UserSession.hpp"
#include "ui/BabelMainWindow.hpp"
#include "NotificationHandler.hpp"
#include "InputOptionsHandler.hpp"

class AppManager {
public:
    typedef void (AppManager::*commandPtr)(std::map<std::string, void*>&);

    explicit AppManager(int argc, char *argv[]);
    ~AppManager() = default;

    void start();

private:

    class AppManagerObserver: public Observer {
    public:
        explicit AppManagerObserver(AppManager *manager);
        ~AppManagerObserver() = default;

        void update(std::map<std::string, void*>) final;

        AppManager *manager_;
    };

    struct FriendInfo {
        std::string username;
        std::string ipAddress;
        short port;
    };

    void askToLog(std::map<std::string, void*> &userInfo);
    void askToRegister(std::map<std::string, void*> &userInfo);
    void askToFetchFriends(std::map<std::string, void*> &userInfo);
    void askToCall(std::map<std::string, void*> &userInfo);
    void askToAcceptCall(std::map<std::string, void*> &userInfo);
    void askToEndCall(std::map<std::string, void*> &userInfo);
    void askToDisconnect(std::map<std::string, void*> &userInfo);
    void addFriendInfo(std::string username, std::string ip, short port);
    void close(std::map<std::string, void*> &userInfo);
    void call(std::map<std::string, void*> &userInfo);
    void requestFriends(std::map<std::string, void*> &userInfo);
    void receiveCallFromServer(std::map<std::string, void*> &userInfo);
    void receiveCallAcceptServer(std::map<std::string, void*> &userInfo);
    void receiveCallAcceptResponse(std::map<std::string, void*> &userInfo);

    void initNotifications();

    void startSoundConnection(std::string username);
    void runUdpServer(std::string ipAddress, short port);

    static std::vector<std::tuple<std::string, void (AppManager::*)(std::map<std::string, void*>&)>> commands;

    std::string remoteIpAddress_;
    std::string localIpAddress_;
    int port_;
    bool isUdpOnly_;
    boost::shared_ptr<NotificationHandler> notifHandler_;
    boost::shared_ptr<ServerHandler> serverHandler_;
    boost::shared_ptr<SoundServerHandler> soundServerHandler_;
    ui::BabelMainWindow widget_;
    boost::shared_ptr<AppManagerObserver> observer_;
    std::vector<FriendInfo> friendsInfo_;
};

#endif //BABEL_SERVER_APPMANAGER_HPP
