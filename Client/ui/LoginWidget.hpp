//
// Created by Quentin Liardeaux on 9/25/19.
//

#ifndef BABEL_SERVER_LOGINWIDGET_HPP
#define BABEL_SERVER_LOGINWIDGET_HPP

#include <QtWidgets>
#include <QSharedPointer>
#include <iostream>

#include "../NotificationHandler.hpp"

namespace ui {

    struct DefaultUIHeights {
        static const int textBoxHeight = 30;
    };

    class LoginWidget : public QWidget {
    Q_OBJECT

    public:
        explicit LoginWidget(QWidget *parent = nullptr, QSharedPointer<NotificationHandler> notifHandler = nullptr);

    private:
        QSharedPointer<QPushButton> button_;
        QSharedPointer<QLineEdit> usernameLineEdit_;
        QSharedPointer<QLineEdit> passwordLineEdit_;
        QSharedPointer<NotificationHandler> notifHandler_;

    private slots:
        void loginTap();
        void closeTap();
    };
}

#endif //BABEL_SERVER_LOGINWIDGET_HPP
