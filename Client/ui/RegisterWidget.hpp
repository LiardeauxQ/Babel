//
// Created by Quentin Liardeaux on 9/26/19.
//

#ifndef BABEL_SERVER_REGISTERWIDGET_HPP
#define BABEL_SERVER_REGISTERWIDGET_HPP

#include <QtWidgets>
#include "LoginWidget.hpp"

namespace ui {
    class RegisterWidget: public QWidget {
        Q_OBJECT

    public:
        explicit RegisterWidget(QWidget *parent = nullptr);

    private slots:
        void registerTap();
        void closeTap();
    private:
        QSharedPointer <QPushButton> button_;
        QSharedPointer <QPushButton> closeButton_;
        QSharedPointer <QLineEdit> usernameLineEdit_;
        QSharedPointer <QLineEdit> passwordLineEdit_;
        QSharedPointer <QLineEdit> confirmPassLineEdit_;
    };
}

#endif //BABEL_SERVER_REGISTERWIDGET_HPP
