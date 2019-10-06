//
// Created by Quentin Liardeaux on 9/25/19.
//

#ifndef BABEL_SERVER_LOGINWIDGET_HPP
#define BABEL_SERVER_LOGINWIDGET_HPP

#include <QtWidgets>
#include <QSharedPointer>
#include <iostream>
#include <boost/shared_ptr.hpp>

#include "../NotificationHandler.hpp"
#include "../protocol/UserSession.hpp"
#include "protocol.h"

namespace ui {

    struct DefaultUIHeights {
        static const int textBoxHeight = 30;
    };

    class LoginWidget : public QWidget {
    Q_OBJECT

    public:
        explicit LoginWidget(boost::shared_ptr<NotificationHandler> notifHandler, QWidget *parent = nullptr);

    private:

        class LoginObserver: public Observer {
        public:
            LoginObserver(LoginWidget &widget);
            ~LoginObserver() = default;

            void update(std::map<std::string, void*> userInfo) final;

            LoginWidget &widget_;
        };

        void loginEvent() const;

        QSharedPointer<QPushButton> button_;
        QSharedPointer<QLineEdit> usernameLineEdit_;
        QSharedPointer<QLineEdit> passwordLineEdit_;
        boost::shared_ptr<NotificationHandler> notifHandler_;

        boost::shared_ptr<Subject> loginEvent_;
        boost::shared_ptr<LoginObserver> observer_;

    protected slots:
        void loginTap();
        void closeTap();
    };
}

#endif //BABEL_SERVER_LOGINWIDGET_HPP
