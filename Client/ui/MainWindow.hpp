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
        ~MainWindow();

    private slots:
        void initRegisterWidget();
        void initLoginWidget();
    private:
        QSharedPointer<QStackedWidget> stackedWidget_;
        QSharedPointer<QWidget> connectionWidget_;

        void registered();
        void logged();
        void closeRegister();
        void closeLogin();

        void removeLastWidget();
    };
}


#endif //BABEL_SERVER_MAINWINDOW_HPP
