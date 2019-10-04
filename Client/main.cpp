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
    boost::shared_ptr<NotificationHandler> notificationHandler(new NotificationHandler());
    boost::shared_ptr<ServerRequest> serverRequest = ServerRequest::create("127.0.0.1", 1234, notificationHandler);
    AppManager app(serverRequest, notificationHandler);

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
