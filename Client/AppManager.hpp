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
    explicit AppManager(boost::shared_ptr<ServerRequest> request,
            boost::shared_ptr<NotificationHandler> notifHandler);
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

    void askToLog(const std::string &username, const std::string &password);
    void askToRegister(const std::string &username, const std::string &password);
    void call();
    void requestFriends();

    boost::shared_ptr<NotificationHandler> notifHandler_;
    boost::shared_ptr<ServerRequest> request_;
    UserSession session_;
    ui::MainWidget widget_;
    boost::shared_ptr<AppManagerObserver> observer_;
};

#endif //BABEL_SERVER_APPMANAGER_HPP
