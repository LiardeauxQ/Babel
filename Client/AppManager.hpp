//
// Created by Quentin Liardeaux on 10/3/19.
//

#ifndef BABEL_SERVER_APPMANAGER_HPP
#define BABEL_SERVER_APPMANAGER_HPP

#include <memory>

#include "protocol/ServerRequest.hpp"
#include "protocol/UserSession.hpp"
#include "ui/MainWidget.hpp"
#include "NotificationHandler.hpp"

class AppManager {
public:
    AppManager(std::unique_ptr<ServerRequest> request);
    ~AppManager() = default;

    void start();
private:
    void askToLog();
    void askToRegister();
    void call();
    void requestFriends();

    std::unique_ptr<ServerRequest> request_;
    UserSession session_;
    NotificationHandler notifHandler_;
    ui::MainWidget widget_;
};

#endif //BABEL_SERVER_APPMANAGER_HPP
