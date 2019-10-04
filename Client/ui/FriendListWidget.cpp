//
// Created by Quentin Liardeaux on 9/27/19.
//

#include "FriendListWidget.hpp"
#include "BabelMainWindow.hpp"

ui::FriendListWidget::FriendListWidget(QWidget *parent) : QWidget(parent)
{
    friendList_ =new QListWidget();
    usernameLabel_ = new QLabel("Quentin Liardeaux");
    disconnectButton_ = new QPushButton("disconnect");
    userProfilWidget_ = QSharedPointer<QWidget>(new QWidget());
    widgetsHandler_ = new WidgetsHandler();

    connect(disconnectButton_, SIGNAL(clicked()), this, SLOT(disconnectTap()));
    connect(friendList_, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(selectListItem(QListWidgetItem *)));

    friendList_->addItem("copain 1");
    friendList_->addItem("copain 2");
    friendList_->addItem("copain 3");
    friendList_->addItem("copain 4");

    QPointer<QGridLayout> layout = new QGridLayout();
    QPointer<QGridLayout> mainLayout = new QGridLayout();

    userProfilWidget_->setLayout(layout);
    userProfilWidget_->layout()->addWidget(usernameLabel_);
    userProfilWidget_->layout()->addWidget(friendList_);
    userProfilWidget_->layout()->addWidget(disconnectButton_);
    widgetsHandler_->push(userProfilWidget_.get());

    mainLayout->addWidget(widgetsHandler_);
    setLayout(mainLayout);
}

void ui::FriendListWidget::disconnectTap()
{
    for (auto action : actions()) {
        if (action->text() == "disconnect") {
            action->trigger();
            break;
        }
    }
}

void ui::FriendListWidget::selectListItem(QListWidgetItem *item)
{
    QPointer<CallWidget> wCall = new CallWidget();
    QPointer<QAction> closeAction = new QAction("close");

    std::cout << "list" << std::endl;
    connect(closeAction, &QAction::triggered, this, &ui::FriendListWidget::stopCall);
    wCall->addAction(closeAction);
    widgetsHandler_->replaceLastWidget(wCall);
}

void ui::FriendListWidget::stopCall()
{
    std::cout << "stop 1" << std::endl;
    widgetsHandler_->replaceLastWidget(userProfilWidget_.get());
}
