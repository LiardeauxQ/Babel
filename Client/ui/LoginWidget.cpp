//
// Created by Quentin Liardeaux on 9/25/19.
//

#include "LoginWidget.hpp"

ui::LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent)
{
    button_ = QSharedPointer<QPushButton>(new QPushButton(tr("Login")));
    usernameTextEdit_ = QSharedPointer<QTextEdit>(new QTextEdit());
    passwordTextEdit_ = QSharedPointer<QTextEdit>(new QTextEdit());

    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout();

    usernameTextEdit_->setFixedHeight(ui::DefaultUIHeights::textBoxHeight);
    usernameTextEdit_->setLineWrapMode(QTextEdit::NoWrap);
    passwordTextEdit_->setFixedHeight(ui::DefaultUIHeights::textBoxHeight);
    passwordTextEdit_->setLineWrapMode(QTextEdit::NoWrap);
    mainLayout->addWidget(usernameTextEdit_.get());
    mainLayout->addWidget(passwordTextEdit_.get());
    mainLayout->addWidget(button_.get());

    setLayout(mainLayout);
    setWindowTitle(tr("Login to Babel"));
}
