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

float CubicAmplifier( float input )
{
    float output, temp;
    if( input < 0.0 )
    {
        temp = input + 1.0f;
        output = (temp * temp * temp) - 1.0f;
    }
    else
    {
        temp = input - 1.0f;
        output = (temp * temp * temp) + 1.0f;
    }

    return output;
}
#define FUZZ(x) CubicAmplifier(CubicAmplifier(CubicAmplifier(CubicAmplifier(x))))

static int fuzzCallback(const void *inputBuffer, void *outputBuffer,
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
            *out++ = *in++;          /* right - clean */
        }
    }

    return paContinue;
}

int callBack(const void *input, void *output, unsigned long frameCount,
             const PaStreamCallbackTimeInfo *timeInfo,
             PaStreamCallbackFlags statusFlags, void *userData) {
    auto inputFloat = (float *) input;

    for (int i = 0; i < frameCount; i++) {
        std::cout << inputFloat[i] << std::endl;
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

        auto stream = audioController.createCustomStream(&out, &in, fuzzCallback, nullptr, paNoFlag, "Distorder");

        stream.start();

        getchar();

        stream.stop();
    } catch (const AudioControllerError &e) {
        std::cerr << "Cannot create controller " << e.what() << std::endl;
        return 1;
    };
    return 0;
}
