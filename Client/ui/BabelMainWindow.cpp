//
// Created by Quentin Liardeaux on 9/26/19.
//

#include "MainWidget.hpp"

#include <utility>

ui::MainWidget::MainWidget(boost::shared_ptr<NotificationHandler> notifHandler, QWidget *parent) :
    QMainWindow(parent),
    notifHandler_(notifHandler)
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
}

void ui::MainWidget::initRegisterWidget()
{
    QPointer<RegisterWidget> wRegister = new RegisterWidget(notifHandler_, nullptr);
    QPointer<QAction> registerAction = new QAction("register");
    QPointer<QAction> closeAction = new QAction("close");

    connect(registerAction, &QAction::triggered, this, &ui::MainWidget::initFriendListWidget);
    wRegister->addAction(registerAction);
    connect(closeAction,&QAction::triggered, this, &ui::MainWidget::returnToConnectionWidget);
    wRegister->addAction(closeAction);
    widgetsHandler_->replaceLastWidget(wRegister);
}

void ui::MainWidget::initLoginWidget()
{
    auto wLogin = new LoginWidget(notifHandler_, nullptr);
    auto loginAction = new QAction("login");
    auto closeAction = new QAction("close");

    connect(loginAction, &QAction::triggered, this, &ui::MainWidget::initFriendListWidget);
    wLogin->addAction(loginAction);
    connect(closeAction, &QAction::triggered, this, &ui::MainWidget::returnToConnectionWidget);
    wLogin->addAction(closeAction);
    widgetsHandler_->replaceLastWidget(wLogin);
}

void ui::MainWidget::initFriendListWidget()
{
    auto wFriendList = new FriendListWidget();
    auto disconnectAction = new QAction("disconnect");

    connect(disconnectAction, &QAction::triggered, this, &ui::MainWidget::returnToConnectionWidget);
    wFriendList->addAction(disconnectAction);
    widgetsHandler_->replaceLastWidget(wFriendList);
}

void ui::MainWidget::registered()
{
    std::cout << "test register" << std::endl;
}

void ui::MainWidget::logged()
{
    std::cout << "test loggin" << std::endl;
}

void ui::MainWidget::returnToConnectionWidget()
{
    widgetsHandler_->replaceLastWidget(connectionWidget_.get());
}

void ui::MainWidget::closeEvent(QCloseEvent *event)
{
    std::cout << "test" << std::endl;
}
