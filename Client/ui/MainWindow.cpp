//
// Created by Quentin Liardeaux on 9/26/19.
//

#include "MainWindow.hpp"

ui::MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    connectionWidget_ = QSharedPointer<QWidget>(new QWidget());
    widgetsHandler_ = new WidgetsHandler();

    QPointer<QPushButton> loginButton = new QPushButton(tr("login"));
    QPointer<QPushButton> registerButton = new QPushButton(tr("register"));

    connect(registerButton, SIGNAL(clicked()), this, SLOT(initRegisterWidget()));
    connect(loginButton, SIGNAL(clicked()), this, SLOT(initLoginWidget()));

    QPointer<QLayout> mainLayout = new QGridLayout();

    connectionWidget_->setLayout(new QGridLayout());
    connectionWidget_->layout()->addWidget(registerButton);
    connectionWidget_->layout()->addWidget(loginButton);
    widgetsHandler_->push(connectionWidget_.get());
    mainLayout->addWidget(widgetsHandler_);
    setLayout(mainLayout);
}

void ui::MainWindow::initRegisterWidget()
{
    QPointer<RegisterWidget> wRegister = new RegisterWidget();
    QPointer<QAction> registerAction = new QAction("register");
    QPointer<QAction> closeAction = new QAction("close");

    connect(registerAction, &QAction::triggered, this, &ui::MainWindow::initFriendListWidget);
    wRegister->addAction(registerAction);
    connect(closeAction,&QAction::triggered, this, &ui::MainWindow::returnToConnectionWidget);
    wRegister->addAction(closeAction);
    widgetsHandler_->replaceLastWidget(wRegister);
}

void ui::MainWindow::initLoginWidget()
{
    auto wLogin = new LoginWidget();
    auto loginAction = new QAction("login");
    auto closeAction = new QAction("close");

    connect(loginAction, &QAction::triggered, this, &ui::MainWindow::initFriendListWidget);
    wLogin->addAction(loginAction);
    connect(closeAction, &QAction::triggered, this, &ui::MainWindow::returnToConnectionWidget);
    wLogin->addAction(closeAction);
    widgetsHandler_->replaceLastWidget(wLogin);
}

void ui::MainWindow::initFriendListWidget()
{
    auto wFriendList = new FriendListWidget();
    auto disconnectAction = new QAction("disconnect");

    connect(disconnectAction, &QAction::triggered, this, &ui::MainWindow::returnToConnectionWidget);
    wFriendList->addAction(disconnectAction);
    widgetsHandler_->replaceLastWidget(wFriendList);
}

void ui::MainWindow::registered()
{
    std::cout << "test register" << std::endl;
}

void ui::MainWindow::logged()
{
    std::cout << "test loggin" << std::endl;
}

void ui::MainWindow::returnToConnectionWidget()
{
    widgetsHandler_->replaceLastWidget(connectionWidget_.get());
}
