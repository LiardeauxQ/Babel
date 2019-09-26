//
// Created by Quentin Liardeaux on 9/26/19.
//

#include "MainWindow.hpp"

ui::MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    stackedWidget_ = QSharedPointer<QStackedWidget>(new QStackedWidget());
    registerButton_ = QSharedPointer<QPushButton>(new QPushButton(tr("register")));
    loginButton_ = QSharedPointer<QPushButton>(new QPushButton(tr("login")));

    connect(registerButton_.get(), SIGNAL(clicked()), this, SLOT(initRegisterWidget()));
    connect(loginButton_.get(), SIGNAL(clicked()), this, SLOT(initLoginWidget()));

    auto layout = QSharedPointer<QGridLayout>(new QGridLayout());

    layout->addWidget(registerButton_.get());
    layout->addWidget(loginButton_.get());


    setLayout(layout.get());
}

void ui::MainWindow::initRegisterWidget()
{
    QPointer<RegisterWidget> wRegister = new RegisterWidget();
    QPointer<QAction> registerAction = new QAction("register");
    QPointer<QAction> closeAction = new QAction("close");

    connect(registerAction, &QAction::triggered, this, &ui::MainWindow::registered);
    wRegister->addAction(registerAction);
    connect(closeAction,&QAction::triggered, this, &ui::MainWindow::closeRegister);
    wRegister->addAction(closeAction);
    stackedWidget_->addWidget(wRegister);
}

void ui::MainWindow::initLoginWidget()
{
    QPointer<LoginWidget> wLogin = new LoginWidget();
    QPointer<QAction> loginAction = new QAction("login");
    QPointer<QAction> closeAction = new QAction("close");

    connect(loginAction, &QAction::triggered, this, &ui::MainWindow::logged);
    wLogin->addAction(loginAction);
    connect(closeAction, &QAction::triggered, this, &ui::MainWindow::closeLogin);
    wLogin->addAction(closeAction);
    layout()->addWidget(wLogin);
    stackedWidget_->addWidget(wLogin);
}

void ui::MainWindow::registered()
{
    std::cout << "test register" << std::endl;
}

void ui::MainWindow::logged()
{
    std::cout << "test loggin" << std::endl;
}

void ui::MainWindow::closeRegister()
{
    std::cout << "close register" << std::endl;
}

void ui::MainWindow::closeLogin()
{
    std::cout << "close login" << std::endl;
}