//
// Created by Quentin Liardeaux on 9/26/19.
//

#ifndef BABEL_SERVER_MAINWINDOW_HPP
#define BABEL_SERVER_MAINWINDOW_HPP

#include <QTWidgets>
#include <QSharedMemory>

#include "RegisterWidget.hpp"
#include "LoginWidget.hpp"

namespace ui {
    class MainWindow : public QWidget {
        Q_OBJECT
    public:
        explicit MainWindow(QWidget *parent = nullptr);

    private slots:
        void initRegisterWidget();
        void initLoginWidget();
    private:
        QSharedPointer<QPushButton> registerButton_;
        QSharedPointer<QPushButton> loginButton_;
        QSharedPointer<QStackedWidget> stackedWidget_;

        void registered();
        void logged();
        void closeRegister();
        void closeLogin();
    };
}


#endif //BABEL_SERVER_MAINWINDOW_HPP
