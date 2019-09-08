//
// Created by alex on 9/8/19.
//

#include <iostream>
#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget window;

    QPushButton button("Welcome to babel!", &window);
    window.setFixedSize(1600, 900);

    button.show();
    window.show();
    return QApplication::exec();
}