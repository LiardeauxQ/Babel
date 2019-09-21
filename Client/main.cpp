//
// Created by alex on 9/8/19.
//

#include "AudioController.hpp"
#include "AudioControllerError.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    try {
        AudioController audioController;

        PaStreamParameters in;
        in.device = audioController.getDefaultInputId();
        in.hostApiSpecificStreamInfo = nullptr;
        in.suggestedLatency = audioController.getDefaultInputDevice()->defaultHighInputLatency;
        in.sampleFormat = paFloat32;
        in.channelCount = 2;

        PaStreamParameters out;
        out.device = audioController.getDefaultOutputId();
        out.hostApiSpecificStreamInfo = nullptr;
        out.suggestedLatency = audioController.getDefaultOutputDevice()->defaultHighOutputLatency;
        out.sampleFormat = paFloat32;
        out.channelCount = 2;

        auto soundManager = audioController.createManager(&in, &out, 44100);

        soundManager->start();

        for (;;) {
            auto data = soundManager->read();

            if (data) {
                std::vector<float> vec(data->begin(), data->end());
                std::reverse(vec.begin(), vec.end());
                soundManager->write(vec);
            }
        }

        soundManager->stop();
    } catch (const AudioControllerError& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    };
    return 0;
}
