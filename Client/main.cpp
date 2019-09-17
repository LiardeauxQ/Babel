//
// Created by alex on 9/8/19.
//

#include "AudioController.hpp"
#include "SoundStream.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>

struct UserData {
    char *data;
    unsigned long offset;
};

static int callBack(const void *inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo *timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *userData) {
    auto *out = (float *) outputBuffer;
    const auto *in = (const float *) inputBuffer;
    unsigned int i;

    if (inputBuffer == nullptr) {
        for (i = 0; i < framesPerBuffer; i++) {
            *out++ = 0;  /* left - silent */
            *out++ = 0;  /* right - silent */
        }
    } else {
        for (i = 0; i < framesPerBuffer; i++) {
            *out++ = *in++;  /* left - distorted */
            *out++ = *in++;  /* right - clean */
        }
    }
    return paContinue;
}

int main(int argc, char *argv[]) {
    try {
        AudioController audioController;
        auto defaultDevice = audioController.getDefaultInputDevice();

        if (!defaultDevice) {
            std::cerr << "No default device" << std::endl;
            return 1;
        }

        PaStreamParameters out;
        out.device = audioController.getDefaultInputId();
        out.channelCount = 2;
        out.sampleFormat = paFloat32;
        out.suggestedLatency = defaultDevice->defaultLowInputLatency;
        out.hostApiSpecificStreamInfo = nullptr;

        auto defaultOutputDevice = audioController.getDefaultInputDevice();

        if (!defaultOutputDevice) {
            std::cerr << "No default device" << std::endl;
            return 1;
        }

        PaStreamParameters in;
        in.device = audioController.getDefaultOutputId();
        in.channelCount = 2;
        in.sampleFormat = paFloat32;
        in.suggestedLatency = defaultOutputDevice->defaultLowOutputLatency;
        in.hostApiSpecificStreamInfo = nullptr;

        auto stream = audioController.createCustomStream(&out, &in, callBack, nullptr, paNoFlag, "Releave");

        stream.start();

        getchar();

        stream.stop();
    } catch (const AudioControllerError &e) {
        std::cerr << "Cannot create controller " << e.what() << std::endl;
        return 1;
    };
    return 0;
}
