//
// Created by Quentin Liardeaux on 9/27/19.
//

#ifndef BABEL_SERVER_FRIENDLISTWIDGET_HPP
#define BABEL_SERVER_FRIENDLISTWIDGET_HPP

#include <QtWidgets>
#include <QSharedPointer>
#include <iostream>

namespace ui {
    class FriendListWidget : public QWidget {
    Q_OBJECT
    public:
        explicit FriendListWidget(QWidget *parent = nullptr);

    private:
        QSharedPointer<QListWidget> friendList_;
        QSharedPointer<QLabel> usernameLabel_;
        QSharedPointer<QPushButton> disconnectButton_;

    private slots:
        void disconnectTap();
    };
}

#endif //BABEL_SERVER_FRIENDLISTWIDGET_HPP
