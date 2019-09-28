//
// Created by Quentin Liardeaux on 9/26/19.
//

#ifndef BABEL_SERVER_MAINWINDOW_HPP
#define BABEL_SERVER_MAINWINDOW_HPP

#include <QtWidgets>
#include <QSharedMemory>

#include "RegisterWidget.hpp"
#include "LoginWidget.hpp"
#include "FriendListWidget.hpp"
#include "WidgetsHandler.hpp"

namespace ui {
    class MainWindow : public QWidget {
        Q_OBJECT
    public:
        explicit MainWindow(QWidget *parent = nullptr);

    private slots:
        void initRegisterWidget();
        void initLoginWidget();
    private:
        QSharedPointer<QWidget> connectionWidget_;
        QPointer<WidgetsHandler> widgetsHandler_;

        void registered();
        void logged();
        void returnToConnectionWidget();

        void initFriendListWidget();
    };
}


#endif //BABEL_SERVER_MAINWINDOW_HPP
