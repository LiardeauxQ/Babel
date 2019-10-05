//
// Created by Quentin Liardeaux on 9/26/19.
//

#include "BabelMainWindow.hpp"

#include <utility>

ui::BabelMainWindow::BabelMainWindow(boost::shared_ptr<NotificationHandler> notifHandler, QWidget *parent) :
    QMainWindow(parent),
    notifHandler_(notifHandler),
    closeEvent_(new Subject("close"))
{
    connectionWidget_ = QSharedPointer<QWidget>(new QWidget());
    widgetsHandler_ = new WidgetsHandler();

    QPointer<QPushButton> loginButton = new QPushButton(tr("login"));
    QPointer<QPushButton> registerButton = new QPushButton(tr("register"));

    connect(registerButton, SIGNAL(clicked()), this, SLOT(initRegisterWidget()));
    connect(loginButton, SIGNAL(clicked()), this, SLOT(initLoginWidget()));

    connectionWidget_->setLayout(new QGridLayout());
    connectionWidget_->layout()->addWidget(registerButton);
    connectionWidget_->layout()->addWidget(loginButton);
    widgetsHandler_->push(connectionWidget_.get());
    setCentralWidget(widgetsHandler_);
    notifHandler_->registerEvent(closeEvent_);
}

void ui::BabelMainWindow::initRegisterWidget()
{
    QPointer<RegisterWidget> wRegister = new RegisterWidget(notifHandler_, nullptr);
    QPointer<QAction> registerAction = new QAction("register");
    QPointer<QAction> closeAction = new QAction("close");

    connect(registerAction, &QAction::triggered, this, &ui::BabelMainWindow::initFriendListWidget);
    wRegister->addAction(registerAction);
    connect(closeAction,&QAction::triggered, this, &ui::BabelMainWindow::returnToConnectionWidget);
    wRegister->addAction(closeAction);
    widgetsHandler_->replaceLastWidget(wRegister);
}

void ui::BabelMainWindow::initLoginWidget()
{
    auto wLogin = new LoginWidget(notifHandler_, nullptr);
    auto loginAction = new QAction("login");
    auto closeAction = new QAction("close");

    connect(loginAction, &QAction::triggered, this, &ui::BabelMainWindow::initFriendListWidget);
    wLogin->addAction(loginAction);
    connect(closeAction, &QAction::triggered, this, &ui::BabelMainWindow::returnToConnectionWidget);
    wLogin->addAction(closeAction);
    widgetsHandler_->replaceLastWidget(wLogin);
}

void ui::BabelMainWindow::initFriendListWidget()
{
    auto wFriendList = new FriendListWidget(notifHandler_);
    auto disconnectAction = new QAction("disconnect");

    connect(disconnectAction, &QAction::triggered, this, &ui::BabelMainWindow::returnToConnectionWidget);
    wFriendList->addAction(disconnectAction);
    widgetsHandler_->replaceLastWidget(wFriendList);
}

void ui::BabelMainWindow::registered()
{
    std::cout << "test register" << std::endl;
}

void ui::BabelMainWindow::logged()
{
    std::cout << "test loggin" << std::endl;
}

void ui::BabelMainWindow::returnToConnectionWidget()
{
    widgetsHandler_->replaceLastWidget(connectionWidget_.get());
}

void ui::BabelMainWindow::closeEvent(QCloseEvent *event)
{
    std::map<std::string, void*> userInfo;

    std::cout << "close" << std::endl;
    userInfo["type"] = (void*)(std::string("close").c_str());
    closeEvent_->notify(userInfo);
}
