//
// Created by Quentin Liardeaux on 9/27/19.
//

#include "WidgetsHandler.hpp"

ui::WidgetsHandler::WidgetsHandler(QLayout *layout, QWidget *parent) : QWidget(parent)
{
    QPointer<QLayout> mainLayout = (layout) ? layout : new QGridLayout();

    stackedWidget_ = new QStackedWidget();//QSharedPointer<QStackedWidget>(new QStackedWidget());

    mainLayout->addWidget(stackedWidget_);
    setLayout(mainLayout);
}

void ui::WidgetsHandler::push(QWidget *widget)
{
    stackedWidget_->addWidget(widget);
}

void ui::WidgetsHandler::replaceLastWidget(QWidget *widget)
{
    popLastWidget();
    stackedWidget_->addWidget(widget);
}

void ui::WidgetsHandler::popLastWidget()
{
    if (stackedWidget_->count() <= 0) {
        return;
    }

    auto lastWidget = stackedWidget_->widget(stackedWidget_->currentIndex());

    stackedWidget_->removeWidget(lastWidget);
}

void ui::WidgetsHandler::pop(QWidget *widget)
{
    stackedWidget_->removeWidget(widget);
}