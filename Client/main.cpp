//
// Created by alex on 9/8/19.
//

#include "AudioController.hpp"
#include <QApplication>
#include "protocol/ServerRequest.hpp"
#include "AppManager.hpp"
#include <memory>

int main(int argc, char *argv[])
{
    QApplication babelApp(argc, argv);
    AppManager app(std::make_unique<ServerRequest>("127.0.0.1", 1234));

    app.start();
    return babelApp.exec();
}

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
