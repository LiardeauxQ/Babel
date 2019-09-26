//
// Created by Quentin Liardeaux on 9/25/19.
//

#include "LoginWidget.hpp"

ui::LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent)
{
    button_ = QSharedPointer<QPushButton>(new QPushButton(tr("Login")));
    usernameLineEdit_ = QSharedPointer<QLineEdit>(new QLineEdit());
    passwordLineEdit_ = QSharedPointer<QLineEdit>(new QLineEdit());

    QPointer<QLabel> usernameLabel = new QLabel(tr("Username:"));
    QPointer<QLabel> passwordLabel = new QLabel(tr("Password:"));

    passwordLineEdit_->setEchoMode(QLineEdit::Password);

    QPointer<QFormLayout> formLayout = new QFormLayout();

    formLayout->addRow(usernameLabel, usernameLineEdit_.get());
    formLayout->addRow(passwordLabel, passwordLineEdit_.get());
    formLayout->addRow(button_.get());

    setLayout(formLayout);
    setWindowTitle(tr("Login to Babel"));
}
