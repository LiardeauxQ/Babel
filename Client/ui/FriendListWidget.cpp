//
// Created by Quentin Liardeaux on 9/27/19.
//

#include "FriendListWidget.hpp"
#include "BabelMainWindow.hpp"

ui::FriendListWidget::FriendListWidget(boost::shared_ptr<NotificationHandler> notifHandler, QWidget *parent) :
    QWidget(parent),
    notifHandler_(notifHandler),
    fetchFriendsEvent_(new Subject("fetchFriends")),
    observer_(new FriendListObserver(*this))
{
    friendList_ =new QListWidget();
    usernameLabel_ = new QLabel("");
    addFriendButton_ = new QPushButton("+");
    disconnectButton_ = new QPushButton("disconnect");
    callWidget_ = QSharedPointer<CallWidget>(new CallWidget(notifHandler_));
    userProfilWidget_ = QSharedPointer<QWidget>(new QWidget());
    widgetsHandler_ = new WidgetsHandler();

    connect(addFriendButton_, SIGNAL(clicked()), this, SLOT(addFriendTap()));
    connect(disconnectButton_, SIGNAL(clicked()), this, SLOT(disconnectTap()));
    connect(friendList_, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(selectListItem(QListWidgetItem *)));

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

    // Set close action of CallWidget
    QPointer<QAction> closeAction = new QAction("close");

    connect(closeAction, &QAction::triggered, this, &ui::FriendListWidget::stopCall);
    callWidget_->addAction(closeAction);

    notifHandler_->registerEvent(fetchFriendsEvent_);
    notifHandler->attachToEvent(observer_, "fetchFriendsResponse");
    notifHandler->attachToEvent(observer_, "callAcceptResponse");
    notifHandler->attachToEvent(observer_, "callServer");
    notifHandler->attachToEvent(observer_, "callAcceptServer");
    fetchFriends();
}

void ui::FriendListWidget::fetchFriends()
{
    std::map<std::string, void*> userInfo;

    userInfo["type"] = strdup("fetchFriends");
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
    callWidget_->setFriendUsername(item->text().toStdString());
    widgetsHandler_->replaceLastWidget(callWidget_.get());
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

void ui::FriendListWidget::acceptCallEvent(char username[USERNAME_LEN])
{
    callWidget_->askToAcceptCall(username);
    widgetsHandler_->replaceLastWidget(callWidget_.get());
}

void ui::FriendListWidget::callEvent(char username[USERNAME_LEN])
{
    callWidget_->displayDirectCall(username);
    widgetsHandler_->replaceLastWidget(callWidget_.get());
}

ui::FriendListWidget::FriendListObserver::FriendListObserver(ui::FriendListWidget &widget) :
    widget_(widget)
{
}

void ui::FriendListWidget::FriendListObserver::update(std::map<std::string, void *> userInfo)
{
    char *type = (char*)userInfo.find("type")->second;
    void *payload = userInfo.find("payload")->second;

    if (!strcmp(type, "fetchFriends")) {
        server_friend_status_t *srv = (server_friend_status_t*)payload;

        widget_.fetchFriendsEvent(srv->usernames);
    } else if (!strcmp(type, "callServer")) {
        server_call_t *srv = (server_call_t*)payload;

        std::cout << "call server friend list" << std::endl;
        widget_.acceptCallEvent(srv->username);
    } else if (!strcmp(type, "callAcceptServer")) {
        std::cout << "friend list call accept server" << std::endl;
        server_accept_call_t *srv = (server_accept_call_t*)payload;

        widget_.callEvent(srv->username);
    }
}