//
// Created by Quentin Liardeaux on 10/3/19.
//

#ifndef BABEL_SERVER_APPMANAGER_HPP
#define BABEL_SERVER_APPMANAGER_HPP

#include <memory>
#include <QSharedPointer>

#include "protocol/ServerHandler.hpp"
#include "protocol/SoundServerHandler.hpp"
#include "protocol/UserSession.hpp"
#include "ui/BabelMainWindow.hpp"
#include "NotificationHandler.hpp"

class AppManager {
public:
    explicit AppManager(const std::string &ipAddress, int port);
    ~AppManager() = default;

    void start();

private:

    class AppManagerObserver: public Observer {
    public:
        explicit AppManagerObserver(AppManager &manager);
        ~AppManagerObserver() = default;

        void update(std::map<std::string, void*>) final;

        AppManager &manager_;
    };

    struct FriendInfo {
        std::string username;
        std::string ipAddress;
        short port;
    };

    void askToLog(const std::string &username, const std::string &password);
    void askToRegister(const std::string &username, const std::string &password);
    void askToFetchFriends();
    void askToCall(const std::string &username);
    void askToAcceptCall(const std::string &username);
    void addFriendInfo(const std::string &username,
            const std::string &ipAddress, short port);
    void startSoundUdpServer(const std::string &username);
    void close();
    void call();
    void requestFriends();

    void initNotifications();

    std::string ipAddress_;
    int port_;
    boost::shared_ptr<NotificationHandler> notifHandler_;
    boost::shared_ptr<ServerHandler> serverHandler_;
    boost::shared_ptr<SoundServerHandler> soundServerHandler_;
    ui::BabelMainWindow widget_;
    boost::shared_ptr<AppManagerObserver> observer_;
    std::vector<FriendInfo> friendsInfo_;
};

#endif //BABEL_SERVER_APPMANAGER_HPP
