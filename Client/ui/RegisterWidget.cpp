//
// Created by Quentin Liardeaux on 9/26/19.
//

#include "RegisterWidget.hpp"

ui::RegisterWidget::RegisterWidget(QWidget *parent) : QWidget(parent)
{
    button_ = QSharedPointer<QPushButton>(new QPushButton(tr("Register")));
    usernameLineEdit_ = QSharedPointer<QLineEdit>(new QLineEdit());
    passwordLineEdit_ = QSharedPointer<QLineEdit>(new QLineEdit());
    confirmPassLineEdit_ = QSharedPointer<QLineEdit>(new QLineEdit());

    QPointer<QLabel> usernameLabel = new QLabel(tr("Username:"));
    QPointer<QLabel> passwordLabel = new QLabel(tr("Password:"));
    QPointer<QLabel> confirmPassLabel = new QLabel(tr("Confirm password:"));

    passwordLineEdit_->setEchoMode(QLineEdit::Password);
    confirmPassLineEdit_->setEchoMode(QLineEdit::Password);

    QPointer<QFormLayout> formLayout = new QFormLayout();

    formLayout->addRow(usernameLabel, usernameLineEdit_.get());
    formLayout->addRow(passwordLabel, passwordLineEdit_.get());
    formLayout->addRow(confirmPassLabel, confirmPassLineEdit_.get());
    formLayout->addRow(button_.get());

    setLayout(formLayout);
    setWindowTitle(tr("Register to Babel"));
}
