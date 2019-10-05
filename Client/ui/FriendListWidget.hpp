//
// Created by Quentin Liardeaux on 9/27/19.
//

#ifndef BABEL_SERVER_FRIENDLISTWIDGET_HPP
#define BABEL_SERVER_FRIENDLISTWIDGET_HPP

#include <QtWidgets>
#include <QSharedPointer>
#include <iostream>
#include <boost/shared_ptr.hpp>

#include "WidgetsHandler.hpp"
#include "CallWidget.hpp"
#include "AddFriendWidget.hpp"
#include "../NotificationHandler.hpp"
#include "../protocol/UserSession.hpp"

namespace ui {
    class FriendListWidget : public QWidget {
    Q_OBJECT
    public:
        explicit FriendListWidget(boost::shared_ptr<NotificationHandler> notifHandler, QWidget *parent = nullptr);

    private:
        QPointer<QPushButton> addFriendButton_;
        QPointer<QListWidget> friendList_;
        QPointer<QLabel> usernameLabel_;
        QPointer<QPushButton> disconnectButton_;
        QSharedPointer<QWidget> userProfilWidget_;
        QPointer<WidgetsHandler> widgetsHandler_;
        boost::shared_ptr<NotificationHandler> notifHandler_;

        boost::shared_ptr<Subject> fetchFriendsEvent_;

        void fetchFriends();

    private slots:
        void addFriendTap();
        void disconnectTap();
        void selectListItem(QListWidgetItem *item);
        void stopCall();
        void closeAddFriend();
    };
}

#endif //BABEL_SERVER_FRIENDLISTWIDGET_HPP
