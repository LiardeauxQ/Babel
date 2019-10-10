//
// Created by Quentin Liardeaux on 9/26/19.
//

#include "RegisterWidget.hpp"

ui::RegisterWidget::RegisterWidget(boost::shared_ptr<NotificationHandler> notifHandler, QWidget *parent) :
    QWidget(parent),
    notifHandler_(notifHandler),
    registerEvent_(new Subject("register")),
    observer_(new RegisterObserver(*this))
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
    notifHandler_->registerEvent(registerEvent_);
    notifHandler_->attachToEvent(observer_, "registerResponse");
}

void ui::RegisterWidget::registerTap()
{
    std::map<std::string, void*> userInfo;

    userInfo["type"] = strdup("register");
    userInfo["username"] = strdup(usernameLineEdit_->text().toStdString().c_str());
    userInfo["password"] = strdup(passwordLineEdit_->text().toStdString().c_str());
    registerEvent_->notify(userInfo);
    free(userInfo["type"]);
    free(userInfo["username"]);
    free(userInfo["password"]);
}

void ui::RegisterWidget::registerEvent() const
{
    for (auto action : actions()) {
        if (action->text() == "register") {
            UserSession::get()->connectUser(usernameLineEdit_->text().toStdString());
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

ui::RegisterWidget::RegisterObserver::RegisterObserver(RegisterWidget &widget) :
        widget_(widget)
{
}

void ui::RegisterWidget::RegisterObserver::update(std::map<std::string, void*> userInfo)
{
    auto type = userInfo.find("type")->second;
    auto response = (RESULT*)(userInfo.find("result")->second);

    if (response != nullptr && *response == RESULT::OK)
        widget_.registerEvent();
}