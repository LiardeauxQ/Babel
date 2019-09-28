//
// Created by alex on 9/8/19.
//

#include "AudioController.hpp"
#include "AudioControllerError.hpp"
#include <iostream>
#include <QApplication>
#include "ui/MainWindow.hpp"
#include "protocol/ServerRequest.hpp"
#include <boost/asio.hpp>

int main(int argc, char *argv[])
{
    ServerRequest request("127.0.0.1", 1234);

    request.sendHello("Quentin", "password");
    return 0;
}

/*
int main(int argc, char *argv[])
{
    QApplication babelApp(argc, argv);
    ui::MainWindow w;

    w.resize(500, 500);
    w.show();
    return babelApp.exec();
}*/

/*
int main(int argc, char* argv[])
{
    try {
        AudioController audioController;

        auto soundManager = audioController.createManager();

        soundManager->start();

        std::vector<float> data;
        data.reserve(512);

        while (soundManager->isActive()) {
            soundManager->read(data);

            soundManager->write(data);
            data.clear();
        }

        soundManager->stop();
    } catch (const AudioControllerError& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    };
    return 0;
}*/
