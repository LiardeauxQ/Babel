//
// Created by Quentin Liardeaux on 10/5/19.
//

#include "AddFriendWidget.hpp"

ui::AddFriendWidget::AddFriendWidget(boost::shared_ptr<NotificationHandler> notifHandler, QWidget *parent) :
        QWidget(parent),
        notifHandler_(notifHandler) {
    QPointer<QPushButton> closeButton = new QPushButton(tr("Close"));
    usernameLineEdit_ = QSharedPointer<QLineEdit>(new QLineEdit());

    QPointer<QLabel> usernameLabel = new QLabel(tr("Username:"));
    QPointer<QLabel> passwordLabel = new QLabel(tr("Password:"));

    connect(closeButton, SIGNAL(clicked()), this, SLOT(closeTap()));

    QPointer<QFormLayout> formLayout = new QFormLayout();

    formLayout->addRow(closeButton);
    formLayout->addRow(usernameLabel, usernameLineEdit_.get());

    setLayout(formLayout);
    setWindowTitle(tr("Add a friend"));
}

void ui::AddFriendWidget::closeTap()
{
    for (auto action : actions()) {
        if (action->text() == "close") {
            action->trigger();
            break;
        }
    }
}
