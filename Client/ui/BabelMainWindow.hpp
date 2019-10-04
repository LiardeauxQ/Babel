//
// Created by Quentin Liardeaux on 9/26/19.
//

#ifndef BABEL_SERVER_MAINWIDGET_HPP
#define BABEL_SERVER_MAINWIDGET_HPP

#include <QtWidgets>
#include <QSharedMemory>
#include <boost/shared_ptr.hpp>

#include "RegisterWidget.hpp"
#include "LoginWidget.hpp"
#include "FriendListWidget.hpp"
#include "WidgetsHandler.hpp"
#include "../NotificationHandler.hpp"
#include "../Subject.hpp"

namespace ui {
    class MainWidget : public QMainWindow {
        Q_OBJECT
    public:
        explicit MainWidget(boost::shared_ptr<NotificationHandler> notifHandler, QWidget *parent = nullptr);

    protected:
        void closeEvent(QCloseEvent *event);
    private slots:
        void initRegisterWidget();
        void initLoginWidget();
    private:
        QSharedPointer<QWidget> connectionWidget_;
        QPointer<WidgetsHandler> widgetsHandler_;
        boost::shared_ptr<NotificationHandler> notifHandler_;

        void registered();
        void logged();
        void returnToConnectionWidget();

        void initFriendListWidget();
    };
}


#endif //BABEL_SERVER_MAINWIDGET_HPP
