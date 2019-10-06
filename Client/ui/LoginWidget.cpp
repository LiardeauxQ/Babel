//
// Created by Quentin Liardeaux on 9/25/19.
//

#include "LoginWidget.hpp"

ui::LoginWidget::LoginWidget(boost::shared_ptr<NotificationHandler> notifHandler, QWidget *parent) :
    QWidget(parent),
    notifHandler_(notifHandler),
    loginEvent_(new Subject("login")),
    observer_(new LoginObserver(*this))
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
    notifHandler_->registerEvent(loginEvent_);
    notifHandler_->attachToEvent(observer_, "loginResponse");
}

void ui::LoginWidget::loginTap()
{
    std::map<std::string, void*> userInfo;

    userInfo["type"] = strdup("login");
    userInfo["username"] = strdup(usernameLineEdit_->text().toStdString().c_str());
    userInfo["password"] = strdup(passwordLineEdit_->text().toStdString().c_str());
    loginEvent_->notify(userInfo);
}

void ui::LoginWidget::loginEvent() const
{
    for (auto action : actions()) {
        if (action->text() == "login") {
            UserSession::get()->connectUser(usernameLineEdit_->text().toStdString());
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

ui::LoginWidget::LoginObserver::LoginObserver(LoginWidget &widget) :
    widget_(widget)
{
}

void ui::LoginWidget::LoginObserver::update(std::map<std::string, void*> userInfo)
{
    auto type = userInfo.find("type")->second;
    auto response = (RESULT*)(userInfo.find("result")->second);

    if (response != nullptr && *response == RESULT::OK)
        widget_.loginEvent();
}