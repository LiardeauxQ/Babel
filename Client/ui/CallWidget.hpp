//
// Created by Quentin Liardeaux on 9/27/19.
//

#ifndef BABEL_SERVER_CALLWIDGET_HPP
#define BABEL_SERVER_CALLWIDGET_HPP

#include <QtWidgets>
#include <QSharedPointer>
#include <iostream>
#include <boost/shared_ptr.hpp>

#include "../NotificationHandler.hpp"

namespace ui {
    class CallWidget : public QWidget {
    Q_OBJECT
    public:
        explicit CallWidget(boost::shared_ptr<NotificationHandler> notifHandler, QWidget *parent = nullptr);

        void setFriendUsername(const std::string &username);
        void displayDirectCall(const std::string &username);
        void askToAcceptCall(const std::string &username);
    private:
        QSharedPointer<QPushButton> closeButton_;
        QSharedPointer<QPushButton> startCallButton_;
        QSharedPointer<QPushButton> stopCallButton_;
        QSharedPointer<QPushButton> acceptCallButton_;
        QSharedPointer<QLabel> friendUsernameLabel_;
        boost::shared_ptr<NotificationHandler> notifHandler_;

        boost::shared_ptr<Subject> callEvent_;
        boost::shared_ptr<Subject> acceptCallEvent_;
        boost::shared_ptr<Subject> endCallEvent_;

        bool isCalling_;
        std::string friendUsername_;
    private slots:
        void closeTap();
        void startTap();
        void stopTap();
        void acceptCallTap();
    };
}

#endif //BABEL_SERVER_CALLWIDGET_HPP
