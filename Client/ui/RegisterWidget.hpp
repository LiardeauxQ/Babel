//
// Created by Quentin Liardeaux on 9/26/19.
//

#ifndef BABEL_SERVER_REGISTERWIDGET_HPP
#define BABEL_SERVER_REGISTERWIDGET_HPP

#include <QtWidgets>
#include "LoginWidget.hpp"
#include "../NotificationHandler.hpp"

namespace ui {
    class RegisterWidget: public QWidget {
        Q_OBJECT

    public:
        explicit RegisterWidget(QWidget *parent = nullptr, QSharedPointer<NotificationHandler> notifHandler = nullptr);

    private slots:
        void registerTap();
        void closeTap();
    private:
        QSharedPointer<QPushButton> button_;
        QSharedPointer<QPushButton> closeButton_;
        QSharedPointer<QLineEdit> usernameLineEdit_;
        QSharedPointer<QLineEdit> passwordLineEdit_;
        QSharedPointer<QLineEdit> confirmPassLineEdit_;
        QSharedPointer<NotificationHandler> notifHandler_;
        Subject registerEvent_;
    };
}

#endif //BABEL_SERVER_REGISTERWIDGET_HPP
