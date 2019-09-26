//
// Created by alex on 9/8/19.
//

#include "AudioController.hpp"
#include "AudioControllerError.hpp"
#include <iostream>
#include <QApplication>
#include "ui/RegisterWidget.hpp"

int main(int argc, char *argv[])
{
    QApplication babelApp(argc, argv);
    ui::LoginWidget w;

    w.show();
    return babelApp.exec();
}

/*
int main(int argc, char* argv[])
{
    try {
        AudioController audioController;

        PaStreamParameters in;
        in.device = audioController.getDefaultInputId();
        in.hostApiSpecificStreamInfo = nullptr;
        in.suggestedLatency = audioController.getDefaultInputDevice()->defaultHighInputLatency;
        in.sampleFormat = paFloat32;
        in.channelCount = 1;

        PaStreamParameters out;
        out.device = audioController.getDefaultOutputId();
        out.hostApiSpecificStreamInfo = nullptr;
        out.suggestedLatency = audioController.getDefaultOutputDevice()->defaultHighOutputLatency;
        out.sampleFormat = paFloat32;
        out.channelCount = 2;

        auto soundManager = audioController.createManager(&in, &out, audioController.getDefaultOutputDevice()->defaultSampleRate);

        soundManager->start();

        for (auto a : audioController.getDevicesInfo()) {
            std::cout << a->name << std::endl;
            std::cout << a->maxOutputChannels << std::endl;
            std::cout << a->maxInputChannels << std::endl;
        }

        for (;;) {
            auto data = soundManager->read();

            if (data) {
                std::vector<float> vec(data->begin(), data->end());
                std::reverse(vec.begin(), vec.end());
                for (auto &e : vec) {
                    //std::cout << e << std::endl;
                }
                soundManager->write(vec);
            }
        }

        soundManager->stop();
    } catch (const AudioControllerError& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    };
    return 0;
}*/
