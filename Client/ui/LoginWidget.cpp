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

    connect(button_.get(), SIGNAL(clicked()), this, SLOT(login()));

    QPointer<QFormLayout> formLayout = new QFormLayout();

    formLayout->addRow(usernameLabel, usernameLineEdit_.get());
    formLayout->addRow(passwordLabel, passwordLineEdit_.get());
    formLayout->addRow(button_.get());

    setLayout(formLayout);
    setWindowTitle(tr("Login to Babel"));
}

void ui::LoginWidget::login()
{
    for (auto action : actions()) {
        if (action->text() == "login") {
            action->trigger();
            break;
        }
    }
}