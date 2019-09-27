//
// Created by Quentin Liardeaux on 9/27/19.
//

#ifndef BABEL_SERVER_FRIENDLISTWIDGET_HPP
#define BABEL_SERVER_FRIENDLISTWIDGET_HPP

#include <QtWidgets>
#include <QSharedPointer>
#include <iostream>

#include "WidgetsHandler.hpp"
#include "CallWidget.hpp"

namespace ui {
    class FriendListWidget : public QWidget {
    Q_OBJECT
    public:
        explicit FriendListWidget(QWidget *parent = nullptr);

    private:
        QPointer<QListWidget> friendList_;
        QPointer<QLabel> usernameLabel_;
        QPointer<QPushButton> disconnectButton_;
        QSharedPointer<QWidget> userProfilWidget_;
        QPointer<WidgetsHandler> widgetsHandler_;

    private slots:
        void disconnectTap();
        void selectListItem(QListWidgetItem *item);
        void stopCall();
    };
}

#endif //BABEL_SERVER_FRIENDLISTWIDGET_HPP
