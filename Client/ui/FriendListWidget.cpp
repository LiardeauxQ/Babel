//
// Created by Quentin Liardeaux on 9/27/19.
//

#include "FriendListWidget.hpp"
#include "BabelMainWindow.hpp"

ui::FriendListWidget::FriendListWidget(boost::shared_ptr<NotificationHandler> notifHandler, QWidget *parent) :
    notifHandler_(notifHandler),
    QWidget(parent),
    fetchFriendsEvent_(new Subject("fetchFriends")),
    observer_(new FriendListObserver(*this))
{
    friendList_ =new QListWidget();
    usernameLabel_ = new QLabel("");
    addFriendButton_ = new QPushButton("+");
    disconnectButton_ = new QPushButton("disconnect");
    userProfilWidget_ = QSharedPointer<QWidget>(new QWidget());
    widgetsHandler_ = new WidgetsHandler();

    connect(addFriendButton_, SIGNAL(clicked()), this, SLOT(addFriendTap()));
    connect(disconnectButton_, SIGNAL(clicked()), this, SLOT(disconnectTap()));
    connect(friendList_, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(selectListItem(QListWidgetItem *)));

    friendList_->addItem("copain 4");

    QPointer<QGridLayout> layout = new QGridLayout();
    QPointer<QGridLayout> mainLayout = new QGridLayout();

    userProfilWidget_->setLayout(layout);
    userProfilWidget_->layout()->addWidget(addFriendButton_);
    userProfilWidget_->layout()->addWidget(usernameLabel_);
    userProfilWidget_->layout()->addWidget(friendList_);
    userProfilWidget_->layout()->addWidget(disconnectButton_);
    widgetsHandler_->push(userProfilWidget_.get());

    if (UserSession::get() != nullptr)
        usernameLabel_->setText(QString(UserSession::get()->getUsername().c_str()));

    mainLayout->addWidget(widgetsHandler_);
    setLayout(mainLayout);

    notifHandler_->registerEvent(fetchFriendsEvent_);
    notifHandler->attachToEvent(observer_, "fetchFriendsResponse");
    fetchFriends();
}

void ui::FriendListWidget::fetchFriends()
{
    std::map<std::string, void*> userInfo;

    userInfo["type"] = (void*)(std::string("fetchFriends").c_str());
    fetchFriendsEvent_->notify(userInfo);
}

void ui::FriendListWidget::addFriendTap()
{
    QPointer<AddFriendWidget> wAddFriend = new AddFriendWidget(notifHandler_);
    QPointer<QAction> closeAction = new QAction("close");

    connect(closeAction, &QAction::triggered, this, &ui::FriendListWidget::closeAddFriend);
    wAddFriend->addAction(closeAction);
    widgetsHandler_->replaceLastWidget(wAddFriend);
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
    QPointer<CallWidget> wCall = new CallWidget(notifHandler_);
    QPointer<QAction> closeAction = new QAction("close");

    wCall->setFriendUsername(item->text().toStdString());
    connect(closeAction, &QAction::triggered, this, &ui::FriendListWidget::stopCall);
    wCall->addAction(closeAction);
    widgetsHandler_->replaceLastWidget(wCall);
}

void ui::FriendListWidget::stopCall()
{
    widgetsHandler_->replaceLastWidget(userProfilWidget_.get());
}

void ui::FriendListWidget::closeAddFriend()
{
    widgetsHandler_->replaceLastWidget(userProfilWidget_.get());
}

void ui::FriendListWidget::fetchFriendsEvent(char usernames[MAX_FRIENDS][USERNAME_LEN])
{
    for (int i = 0 ; i < MAX_FRIENDS ; i++) {
        if (strlen(usernames[i]) > 0)
            friendList_->addItem(usernames[i]);
    }
}

ui::FriendListWidget::FriendListObserver::FriendListObserver(ui::FriendListWidget &widget) :
    widget_(widget)
{
}

void ui::FriendListWidget::FriendListObserver::update(std::map<std::string, void *> userInfo)
{
    auto response = userInfo.find("payload")->second;
    server_friend_status_t *srv = (server_friend_status_t*)response;

    if (srv != nullptr)
        widget_.fetchFriendsEvent(srv->usernames);
}