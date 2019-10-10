//
// Created by alex on 9/8/19.
//

#include "AudioController.hpp"
#include <QApplication>
#include "AppManager.hpp"

int main(int argc, char *argv[])
{
    try {
        QApplication babelApp(argc, argv);
        AppManager app(argc, argv);

        app.start();

        return babelApp.exec();
    } catch (std::exception &e) {
        std::cerr << "Error while starting app." << std::endl;
    }
    return 1;
}