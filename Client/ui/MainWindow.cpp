//
// Created by Quentin Liardeaux on 9/26/19.
//

#include "MainWindow.hpp"

ui::MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    stackedWidget_ = QSharedPointer<QStackedWidget>(new QStackedWidget());
    connectionWidget_ = QSharedPointer<QWidget>(new QWidget());

    QPointer<QPushButton> loginButton = new QPushButton(tr("login"));
    QPointer<QPushButton> registerButton = new QPushButton(tr("register"));

    connect(registerButton, SIGNAL(clicked()), this, SLOT(initRegisterWidget()));
    connect(loginButton, SIGNAL(clicked()), this, SLOT(initLoginWidget()));

    QPointer<QLayout> mainLayout = new QGridLayout();

    connectionWidget_->setLayout(new QGridLayout());
    connectionWidget_->layout()->addWidget(registerButton);
    connectionWidget_->layout()->addWidget(loginButton);
    stackedWidget_->addWidget(connectionWidget_.get());
    mainLayout->addWidget(stackedWidget_.get());
    setLayout(mainLayout);
}

ui::MainWindow::~MainWindow() {
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
    removeLastWidget();
    stackedWidget_->addWidget(wRegister);
    std::cout << "register" << std::endl;
}

void ui::MainWindow::initLoginWidget()
{
    auto wLogin = new LoginWidget();
    auto loginAction = new QAction("login");
    auto closeAction = new QAction("close");

    connect(loginAction, &QAction::triggered, this, &ui::MainWindow::logged);
    wLogin->addAction(loginAction);
    connect(closeAction, &QAction::triggered, this, &ui::MainWindow::closeLogin);
    wLogin->addAction(closeAction);
    removeLastWidget();
    stackedWidget_->addWidget(wLogin);
    std::cout << "login" << std::endl;
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
    removeLastWidget();
    stackedWidget_->addWidget(connectionWidget_.get());
}

void ui::MainWindow::closeLogin()
{
    removeLastWidget();
    stackedWidget_->addWidget(connectionWidget_.get());
}

void ui::MainWindow::removeLastWidget()
{
    if (stackedWidget_->count() <= 0) {
        return;
    }

    auto lastWidget = stackedWidget_->widget(stackedWidget_->currentIndex());

    stackedWidget_->removeWidget(lastWidget);
}