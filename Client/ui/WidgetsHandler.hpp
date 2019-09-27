//
// Created by Quentin Liardeaux on 9/27/19.
//

#ifndef BABEL_SERVER_WIDGETSHANDLER_HPP
#define BABEL_SERVER_WIDGETSHANDLER_HPP

#include <QtWidgets>
#include <QSharedPointer>

namespace ui {
    class WidgetsHandler : public QWidget {
    Q_OBJECT
    public:
        explicit WidgetsHandler(QLayout *layout = nullptr, QWidget *parent = nullptr);

        void push(QWidget *widget);
        void replaceLastWidget(QWidget *widget);
        void popLastWidget();
        void pop(QWidget *widget);

    private:
        QPointer<QStackedWidget> stackedWidget_;
    };
}


#endif //BABEL_SERVER_WIDGETSHANDLER_HPP
