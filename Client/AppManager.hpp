//
// Created by Quentin Liardeaux on 10/3/19.
//

#ifndef BABEL_SERVER_APPMANAGER_HPP
#define BABEL_SERVER_APPMANAGER_HPP

#include <memory>
#include <QSharedPointer>

#include "protocol/ServerRequest.hpp"
#include "protocol/UserSession.hpp"
#include "ui/MainWidget.hpp"
#include "NotificationHandler.hpp"

class AppManager {
public:
    explicit AppManager(std::unique_ptr<ServerRequest> request);
    ~AppManager() = default;

    void start();

private:

    class AppManagerObservable: public Observer {
    public:
        explicit AppManagerObservable(AppManager *manager);
        ~AppManagerObservable() = default;

        void update() final;
    private:
        AppManager *manager_;
    };

    void askToLog();
    void askToRegister();
    void call();
    void requestFriends();

    std::unique_ptr<ServerRequest> request_;
    UserSession session_;
    QSharedPointer<NotificationHandler> notifHandler_;
    ui::MainWidget widget_;
};

#endif //BABEL_SERVER_APPMANAGER_HPP
