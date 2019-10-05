//
// Created by Quentin Liardeaux on 9/27/19.
//

#include "CallWidget.hpp"

ui::CallWidget::CallWidget(boost::shared_ptr<NotificationHandler> notifHandler, QWidget *parent) :
    QWidget(parent),
    notifHandler_(notifHandler),
    callEvent_(new Subject("call"))
{
    closeButton_ = QSharedPointer<QPushButton>(new QPushButton("Close"));
    startCallButton_ = QSharedPointer<QPushButton>(new QPushButton("Start"));
    stopCallButton_ = QSharedPointer<QPushButton>(new QPushButton("Stop"));
    friendUsernameLabel_ = QSharedPointer<QLabel>(new QLabel("Calling Quentin..."));

    connect(closeButton_.get(), SIGNAL(clicked()), this, SLOT(closeTap()));
    connect(startCallButton_.get(), SIGNAL(clicked()), this, SLOT(startTap()));
    connect(stopCallButton_.get(), SIGNAL(clicked()), this, SLOT(stopTap()));

    QPointer<QGridLayout> mainLayout = new QGridLayout();

    mainLayout->addWidget(closeButton_.get());
    mainLayout->addWidget(friendUsernameLabel_.get());
    mainLayout->addWidget(startCallButton_.get());
    mainLayout->addWidget(stopCallButton_.get());

    setLayout(mainLayout);
    notifHandler->registerEvent(callEvent_);
}

void ui::CallWidget::setFriendUsername(const std::string &username)
{
    friendUsername_ = username;
    friendUsernameLabel_->setText(QString("Do you want to call ") + QString(friendUsername_.c_str()));
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

    isCalling_ = true;
    friendUsernameLabel_->setText(QString("Calling ") + QString(friendUsername_.c_str()));
    userInfo["type"] = (void*)(std::string("call").c_str());
    userInfo["username"] = (void*)(friendUsername_.c_str());
    callEvent_->notify(userInfo);
}

void ui::CallWidget::stopTap()
{
    isCalling_ = false;
    std::cout << "stop" << std::endl;
}

