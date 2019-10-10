//
// Created by Quentin Liardeaux on 9/27/19.
//

#include "CallWidget.hpp"

ui::CallWidget::CallWidget(boost::shared_ptr<NotificationHandler> notifHandler, QWidget *parent) :
    QWidget(parent),
    notifHandler_(notifHandler),
    callEvent_(new Subject("call")),
    acceptCallEvent_(new Subject("acceptCall")),
    endCallEvent_(new Subject("endCall"))
{
    closeButton_ = QSharedPointer<QPushButton>(new QPushButton("Close"));
    startCallButton_ = QSharedPointer<QPushButton>(new QPushButton("Start"));
    stopCallButton_ = QSharedPointer<QPushButton>(new QPushButton("Stop"));
    acceptCallButton_ = QSharedPointer<QPushButton>(new QPushButton("Accept ?"));
    friendUsernameLabel_ = QSharedPointer<QLabel>(new QLabel("Calling ..."));

    connect(closeButton_.get(), SIGNAL(clicked()), this, SLOT(closeTap()));
    connect(startCallButton_.get(), SIGNAL(clicked()), this, SLOT(startTap()));
    connect(stopCallButton_.get(), SIGNAL(clicked()), this, SLOT(stopTap()));
    connect(acceptCallButton_.get(), SIGNAL(clicked()), this, SLOT(acceptCallTap()));

    QPointer<QGridLayout> mainLayout = new QGridLayout();

    mainLayout->addWidget(closeButton_.get());
    mainLayout->addWidget(friendUsernameLabel_.get());
    mainLayout->addWidget(startCallButton_.get());
    mainLayout->addWidget(stopCallButton_.get());
    mainLayout->addWidget(acceptCallButton_.get());
    acceptCallButton_->setHidden(true);

    setLayout(mainLayout);
    notifHandler->registerEvent(callEvent_);
    notifHandler->registerEvent(acceptCallEvent_);
    notifHandler->registerEvent(endCallEvent_);
}

void ui::CallWidget::setFriendUsername(const std::string &username)
{
    friendUsername_ = username;
    friendUsernameLabel_->setText(QString("Do you want to call ") + QString(friendUsername_.c_str()));
}

void ui::CallWidget::displayDirectCall(const std::string &username)
{
    friendUsername_ = username;
    friendUsernameLabel_->setText(QString("You are in call with ") + QString(friendUsername_.c_str()));
    stopCallButton_->setHidden(false);
    startCallButton_->setHidden(true);
    acceptCallButton_->setHidden(true);
    isCalling_ = true;
}

void ui::CallWidget::askToAcceptCall(const std::string &username)
{
    friendUsername_ = username;
    friendUsernameLabel_->setText(QString("Accept Call from ") + QString(username.c_str()));
    acceptCallButton_->setHidden(false);
    startCallButton_->setHidden(true);
    stopCallButton_->setHidden(true);
}

void ui::CallWidget::closeTap()
{
    for (auto action : actions()) {
        if (action->text() == "close") {
            action->trigger();
            break;
        }
    }
}

void ui::CallWidget::startTap()
{
    std::map<std::string, void*> userInfo;

    friendUsernameLabel_->setText(QString("Calling ") + QString(friendUsername_.c_str()));
    userInfo["type"] = strdup("call");
    userInfo["username"] = strdup(friendUsername_.c_str());
    callEvent_->notify(userInfo);
    free(userInfo["type"]);
    free(userInfo["username"]);
}

void ui::CallWidget::stopTap()
{
    std::map<std::string, void*> userInfo;
    isCalling_ = false;
    startCallButton_->setHidden(false);

    userInfo["type"] = strdup("endCall");
    endCallEvent_->notify(userInfo);
    free(userInfo["type"]);
}

void ui::CallWidget::acceptCallTap()
{
    std::map<std::string, void*> userInfo;

    userInfo["type"] = strdup("acceptCall");
    userInfo["username"] = strdup(friendUsername_.c_str());
    acceptCallEvent_->notify(userInfo);
    free(userInfo["type"]);
    free(userInfo["username"]);
}