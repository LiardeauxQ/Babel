//
// Created by Quentin Liardeaux on 9/25/19.
//

#ifndef BABEL_SERVER_LOGINWIDGET_HPP
#define BABEL_SERVER_LOGINWIDGET_HPP

#include <memory>
#include <QtWidgets>
#include <QSharedPointer>
#include <iostream>

namespace ui {

    struct DefaultUIHeights {
        static const int textBoxHeight = 30;
    };

    class LoginWidget : public QWidget {
    Q_OBJECT

    public:
        explicit LoginWidget(QWidget *parent = nullptr);

    private:
        QSharedPointer<QPushButton> button_;
        QSharedPointer<QLineEdit> usernameLineEdit_;
        QSharedPointer<QLineEdit> passwordLineEdit_;

    private slots:
        void login();
        void close();
    };
}

#endif //BABEL_SERVER_LOGINWIDGET_HPP
