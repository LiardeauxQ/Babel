//
// Created by Quentin Liardeaux on 9/26/19.
//

#ifndef BABEL_SERVER_REGISTERWIDGET_HPP
#define BABEL_SERVER_REGISTERWIDGET_HPP

#include <QtWidgets>
#include <boost/shared_ptr.hpp>

#include "LoginWidget.hpp"
#include "../NotificationHandler.hpp"

namespace ui {
    class RegisterWidget: public QWidget {
        Q_OBJECT

    public:
        explicit RegisterWidget(boost::shared_ptr<NotificationHandler> notifHandler, QWidget *parent = nullptr);

    private slots:
        void registerTap();
        void closeTap();
    private:

        class RegisterObserver: public Observer {
        public:
            RegisterObserver(RegisterWidget &widget);
            ~RegisterObserver() = default;

            void update(std::map<std::string, void*> userInfo) final;

            RegisterWidget &widget_;
        };

        void registerEvent() const;

        QSharedPointer<QPushButton> button_;
        QSharedPointer<QPushButton> closeButton_;
        QSharedPointer<QLineEdit> usernameLineEdit_;
        QSharedPointer<QLineEdit> passwordLineEdit_;
        QSharedPointer<QLineEdit> confirmPassLineEdit_;
        boost::shared_ptr<NotificationHandler> notifHandler_;
        boost::shared_ptr<Subject> registerEvent_;
        boost::shared_ptr<RegisterObserver> observer_;
    };
}

#endif //BABEL_SERVER_REGISTERWIDGET_HPP
