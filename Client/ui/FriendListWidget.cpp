//
// Created by Quentin Liardeaux on 9/27/19.
//

#include "FriendListWidget.hpp"

ui::FriendListWidget::FriendListWidget(QWidget *parent) : QWidget(parent)
{
    friendList_ = QSharedPointer<QListWidget>(new QListWidget());
    usernameLabel_ = QSharedPointer<QLabel>(new QLabel("Quentin Liardeaux"));
    disconnectButton_ = QSharedPointer<QPushButton>(new QPushButton("disconnect"));

    connect(disconnectButton_.get(), SIGNAL(clicked()), this, SLOT(disconnectTap()));

    friendList_->addItem("copain 1");
    friendList_->addItem("copain 2");
    friendList_->addItem("copain 3");
    friendList_->addItem("copain 4");

    QPointer<QGridLayout> mainLayout = new QGridLayout();

    mainLayout->addWidget(usernameLabel_.get());
    mainLayout->addWidget(friendList_.get());
    mainLayout->addWidget(disconnectButton_.get());

    setLayout(mainLayout);
}

void ui::FriendListWidget::disconnectTap()
{
    std::cout << "disconnection" << std::endl;
}