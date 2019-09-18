//
// Created by alex on 9/17/19.
//

#ifndef BABEL_SERVER_SOUNDHANDLER_HPP
#define BABEL_SERVER_SOUNDHANDLER_HPP

#include "AudioController.hpp"
#include "SoundStream.hpp"
#include <boost/circular_buffer.hpp>
#include <queue>

class SoundHandler {
public:
    explicit SoundHandler(AudioController &controller)
            : outputBuffer_()
            , inputBuffer_()
            , inputStream_(
                    controller.createStream(controller.getDefaultInputDevice(), inputCallback_,
                                            static_cast<void *>(&this->inputBuffer_), "InputBuffer")
            )
            , outputStream_(
            controller.createStream(controller.getDefaultOutputDevice(), outputCallback_,
                                    static_cast<void *>(&this->outputBuffer_), "OutputBuffer")
    ) {}

private:
    std::queue<float> inputBuffer_;

    std::queue<float> outputBuffer_;

    // Microphone sound stream that write to inputBuffer_.
    SoundStream inputStream_;

    // Speaker sound stream that read from outputBuffer_.
    SoundStream outputStream_;

    static int outputCallback_(const void *inputBuffer, void *outputBuffer,
                              unsigned long framesPerBuffer,
                              const PaStreamCallbackTimeInfo *timeInfo,
                              PaStreamCallbackFlags statusFlags,
                              void *userData) {
        auto out = (float *) outputBuffer;
        auto in = static_cast<std::queue<float> *>(userData);

        if (in->size() < framesPerBuffer * 2)
            return paContinue;

        for (size_t i = 0; i < framesPerBuffer; ++i) {
            *out++ = in->back();
            in->pop();
            *out++ = in->back();
            in->pop();
        }
        return paContinue;
    }

    static int inputCallback_(const void *inputBuffer, void *outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo *timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void *userData) {
        const auto *in = (const float *) inputBuffer;
        auto out = static_cast<std::queue<float> *>(userData);

        for (size_t i = 0; i < framesPerBuffer; i++) {
            out->push(*in++); // push left.
            out->push(*in++); // push right.
        }
        return paContinue;
    }
};


#endif //BABEL_SERVER_SOUNDHANDLER_HPP
