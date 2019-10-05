//
// Created by Quentin Liardeaux on 10/5/19.
//

#ifndef BABEL_SERVER_ADDFRIENDWIDGET_HPP
#define BABEL_SERVER_ADDFRIENDWIDGET_HPP

#include <QtWidgets>
#include <QSharedPointer>
#include <iostream>
#include <boost/shared_ptr.hpp>

#include "../NotificationHandler.hpp"
#include "protocol.h"

namespace ui {
    class AddFriendWidget: public QWidget {
    Q_OBJECT

    public:
        explicit AddFriendWidget(boost::shared_ptr<NotificationHandler> notifHandler, QWidget *parent = nullptr);

    private:

        QSharedPointer<QLineEdit> usernameLineEdit_;
        boost::shared_ptr<NotificationHandler> notifHandler_;

    protected slots:
        void closeTap();
    };
}

#endif //BABEL_SERVER_ADDFRIENDWIDGET_HPP
