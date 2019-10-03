//
// Created by Quentin Liardeaux on 9/26/19.
//

#include "RegisterWidget.hpp"

ui::RegisterWidget::RegisterWidget(QWidget *parent, QSharedPointer<NotificationHandler> notifHandler) :
    QWidget(parent),
    notifHandler_(notifHandler),
    registerEvent_(Subject("register"))
{
    closeButton_ = QSharedPointer<QPushButton>(new QPushButton(tr("Close")));
    button_ = QSharedPointer<QPushButton>(new QPushButton(tr("Register")));
    usernameLineEdit_ = QSharedPointer<QLineEdit>(new QLineEdit());
    passwordLineEdit_ = QSharedPointer<QLineEdit>(new QLineEdit());
    confirmPassLineEdit_ = QSharedPointer<QLineEdit>(new QLineEdit());

    connect(closeButton_.get(), SIGNAL(clicked()), this, SLOT(closeTap()));
    connect(button_.get(), SIGNAL(clicked()), this, SLOT(registerTap()));

    QPointer<QLabel> usernameLabel = new QLabel(tr("Username:"));
    QPointer<QLabel> passwordLabel = new QLabel(tr("Password:"));
    QPointer<QLabel> confirmPassLabel = new QLabel(tr("Confirm password:"));

    passwordLineEdit_->setEchoMode(QLineEdit::Password);
    confirmPassLineEdit_->setEchoMode(QLineEdit::Password);

    QPointer<QFormLayout> formLayout = new QFormLayout();

    formLayout->addRow(closeButton_.get());
    formLayout->addRow(usernameLabel, usernameLineEdit_.get());
    formLayout->addRow(passwordLabel, passwordLineEdit_.get());
    formLayout->addRow(confirmPassLabel, confirmPassLineEdit_.get());
    formLayout->addRow(button_.get());

    setLayout(formLayout);
    setWindowTitle(tr("Register to Babel"));
    notifHandler_->registerEvent(&registerEvent_);
}

void ui::RegisterWidget::registerTap()
{
    std::map<std::string, void*> userInfo;

    userInfo.insert(std::pair<std::string, void*>(
            std::string("type"),
            (void*)(std::string("register").c_str())));
    userInfo.insert(std::pair<std::string, void*>(
            std::string("username"),
            (void*)(usernameLineEdit_->text().toStdString().c_str())));
    userInfo.insert(std::pair<std::string, void*>(
            std::string("password"),
            (void*)(passwordLineEdit_->text().toStdString().c_str())));
    for (auto action : actions()) {
        if (action->text() == "register") {
            registerEvent_.notify(userInfo);
            action->trigger();
            break;
        }
    }
}

void ui::RegisterWidget::closeTap()
{
    for (auto action : actions()) {
        if (action->text() == "close") {
            action->trigger();
            break;
        }
    }
}