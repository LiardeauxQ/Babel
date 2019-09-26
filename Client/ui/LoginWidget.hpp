//
// Created by Quentin Liardeaux on 9/25/19.
//

#ifndef BABEL_SERVER_LOGINWIDGET_HPP
#define BABEL_SERVER_LOGINWIDGET_HPP

#include <memory>
#include <QMainWindow>
#include <QtWidgets>
#include <QSharedPointer>

namespace ui {

    struct DefaultUIHeights {
        static const int textBoxHeight = 30;
    };

    class LoginWidget : public QWidget {
    Q_OBJECT

    public:
        explicit LoginWidget(QWidget *parent = nullptr);

        ~LoginWidget() override = default;

    private:
        QSharedPointer<QPushButton> button_;
        QSharedPointer<QTextEdit> usernameTextEdit_;
        QSharedPointer<QTextEdit> passwordTextEdit_;
    };
}

#endif //BABEL_SERVER_LOGINWIDGET_HPP
