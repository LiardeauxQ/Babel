//
// Created by Quentin Liardeaux on 9/25/19.
//

#ifndef BABEL_SERVER_MAINWIDGET_HPP
#define BABEL_SERVER_MAINWIDGET_HPP

#include <memory>
#include <QMainWindow>
#include <QtWidgets>

class QPushButton;
class QTextBrowser;

namespace ui {
    class MainWidget : public QWidget {
    Q_OBJECT

    public:
        explicit MainWidget(QWidget *parent = nullptr);

        ~MainWidget() override = default;

    private:
        std::shared_ptr<QPushButton> button_;
        std::shared_ptr<QTextBrowser> textBrowser_;
    };
}

#endif //BABEL_SERVER_MAINWIDGET_HPP
