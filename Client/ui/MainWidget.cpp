//
// Created by Quentin Liardeaux on 9/25/19.
//

#include "MainWidget.hpp"

ui::MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    button_ = std::make_shared<QPushButton>(tr("Try"));
    textBrowser_ = std::make_shared<QTextBrowser>();

    auto mainLayout = std::make_shared<QGridLayout>();

    mainLayout->addWidget(textBrowser_.get());
    mainLayout->addWidget(button_.get());

    setLayout(mainLayout.get());
    setWindowTitle(tr("Test window"));
}
