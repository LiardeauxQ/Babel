//
// Created by Quentin Liardeaux on 9/27/19.
//

#ifndef BABEL_SERVER_CALLWIDGET_HPP
#define BABEL_SERVER_CALLWIDGET_HPP

#include <QtWidgets>
#include <QSharedPointer>
#include <iostream>

namespace ui {
    class CallWidget : public QWidget {
    Q_OBJECT
    public:
        explicit CallWidget(QWidget *parent = nullptr);
    private:
        QSharedPointer<QPushButton> closeButton_;
        QSharedPointer<QPushButton> startCallButton_;
        QSharedPointer<QPushButton> stopCallButton_;
        QSharedPointer<QLabel> friendUsernameLabel_;

    private slots:
        void closeTap();
        void startTap();
        void stopTap();
    };
}


#endif //BABEL_SERVER_CALLWIDGET_HPP
