//
// Created by Quentin Liardeaux on 9/25/19.
//

#include "LoginWidget.hpp"

ui::LoginWidget::LoginWidget(QWidget *parent, QSharedPointer<NotificationHandler> notifHandler) :
    QWidget(parent),
    notifHandler_(notifHandler),
    loginEvent_(Subject("login"))
{
    QPointer<QPushButton> closeButton = new QPushButton(tr("Close"));
    button_ = QSharedPointer<QPushButton>(new QPushButton(tr("Login")));
    usernameLineEdit_ = QSharedPointer<QLineEdit>(new QLineEdit());
    passwordLineEdit_ = QSharedPointer<QLineEdit>(new QLineEdit());

    QPointer<QLabel> usernameLabel = new QLabel(tr("Username:"));
    QPointer<QLabel> passwordLabel = new QLabel(tr("Password:"));

    passwordLineEdit_->setEchoMode(QLineEdit::Password);

    connect(button_.get(), SIGNAL(clicked()), this, SLOT(loginTap()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(closeTap()));

    QPointer<QFormLayout> formLayout = new QFormLayout();

    formLayout->addRow(closeButton);
    formLayout->addRow(usernameLabel, usernameLineEdit_.get());
    formLayout->addRow(passwordLabel, passwordLineEdit_.get());
    formLayout->addRow(button_.get());

    setLayout(formLayout);
    setWindowTitle(tr("Login to Babel"));
    notifHandler_->registerEvent(&loginEvent_);
}

void ui::LoginWidget::loginTap()
{
    std::map<std::string, void*> userInfo;

    userInfo.insert(std::pair<std::string, void*>(std::string("type"),
            (void*)(std::string("login").c_str())));
    userInfo.insert(std::pair<std::string, void*>(
            std::string("username"),
            (void*)(usernameLineEdit_->text().toStdString().c_str())));
    userInfo.insert(std::pair<std::string, void*>(
            std::string("password"),
            (void*)(passwordLineEdit_->text().toStdString().c_str())));
    for (auto action : actions()) {
        if (action->text() == "login") {
            loginEvent_.notify(userInfo);
            action->trigger();
            break;
        }
    }
}

void ui::LoginWidget::closeTap()
{
    for (auto action : actions()) {
        if (action->text() == "close") {
            action->trigger();
            break;
        }
    }
}