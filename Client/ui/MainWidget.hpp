//
// Created by Quentin Liardeaux on 9/26/19.
//

#ifndef BABEL_SERVER_MAINWIDGET_HPP
#define BABEL_SERVER_MAINWIDGET_HPP

#include <QtWidgets>
#include <QSharedMemory>

#include "RegisterWidget.hpp"
#include "LoginWidget.hpp"
#include "FriendListWidget.hpp"
#include "WidgetsHandler.hpp"
#include "../NotificationHandler.hpp"
#include "../Subject.hpp"

namespace ui {
    class MainWidget : public QWidget {
        Q_OBJECT
    public:
        explicit MainWidget(QWidget *parent = nullptr, QSharedPointer<NotificationHandler> notifHandler = nullptr);

    private slots:
        void initRegisterWidget();
        void initLoginWidget();
    private:
        QSharedPointer<QWidget> connectionWidget_;
        QPointer<WidgetsHandler> widgetsHandler_;
        QSharedPointer<NotificationHandler> notifHandler_;

        Subject sub;

        void registered();
        void logged();
        void returnToConnectionWidget();

        void initFriendListWidget();
    };
}


#endif //BABEL_SERVER_MAINWIDGET_HPP
