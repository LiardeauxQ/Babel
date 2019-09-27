//
// Created by Quentin Liardeaux on 9/27/19.
//

#include "CallWidget.hpp"

ui::CallWidget::CallWidget(QWidget *parent) : QWidget(parent)
{
    closeButton_ = QSharedPointer<QPushButton>(new QPushButton("Close"));
    startCallButton_ = QSharedPointer<QPushButton>(new QPushButton("Start"));
    stopCallButton_ = QSharedPointer<QPushButton>(new QPushButton("Stop"));
    friendUsernameLabel_ = QSharedPointer<QLabel>(new QLabel("Calling Quentin..."));

    connect(closeButton_.get(), SIGNAL(clicked()), this, SLOT(closeTap()));
    connect(startCallButton_.get(), SIGNAL(clicked()), this, SLOT(startTap()));
    connect(closeButton_.get(), SIGNAL(clicked()), this, SLOT(stopTap()));

    QPointer<QGridLayout> mainLayout = new QGridLayout();

    mainLayout->addWidget(closeButton_.get());
    mainLayout->addWidget(friendUsernameLabel_.get());
    mainLayout->addWidget(startCallButton_.get());
    mainLayout->addWidget(stopCallButton_.get());

    setLayout(mainLayout);
}

void ui::CallWidget::closeTap()
{
    std::cout << "close" << std::endl;
}

void ui::CallWidget::startTap()
{
    std::cout << "start" << std::endl;
}

void ui::CallWidget::stopTap()
{
    std::cout << "stop" << std::endl;
}

