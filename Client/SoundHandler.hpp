//
// Created by alex on 9/17/19.
//

#ifndef BABEL_SERVER_SOUNDHANDLER_HPP
#define BABEL_SERVER_SOUNDHANDLER_HPP

#include "AudioController.hpp"
#include "SoundStream.hpp"

class SoundHandler {
public:
    explicit SoundHandler(AudioController &controller)
            : inputStream_(
            controller.createStream(controller.getDefaultInputDevice(), inputCallback,
                                    static_cast<void *>(&this->inputData_), "InputBuffer")
    ), outputStream_(
            controller.createStream(controller.getDefaultOutputDevice(), outputCallback,
                                    static_cast<void *>(&this->outputData_), "Output buffer")
    ), outputData_{}, inputData_{} {}

    read();

private:

    static const size_t MAX_INPUT_BUFFER_SIZE = 10000;
    static const size_t MAX_OUTPUT_BUFFER_SIZE = 10000;

    struct CircularBuffer {
        char buffer[MAX_INPUT_BUFFER_SIZE];
        size_t cursor;
    };

    // Microphone sound stream that write to inputBuffer_.
    SoundStream inputStream_;

    // Speaker sound stream that read from outputBuffer_.
    SoundStream outputStream_;

    // Data linked to input stream.
    CircularBuffer inputData_;

    // Data linked to output stream.
    CircularBuffer outputData_;


    static int outputCallback(const void *inputBuffer, void *outputBuffer,
                              unsigned long framesPerBuffer,
                              const PaStreamCallbackTimeInfo *timeInfo,
                              PaStreamCallbackFlags statusFlags,
                              void *userData) {
        auto out = (float *) outputBuffer;
        auto in = static_cast<CircularBuffer *>(userData);

        for (size_t i = 0; i < framesPerBuffer; ++i) {
            *out++ = in->buffer[in->cursor++]; // left
            *out++ = in->buffer[in->cursor++]; // right
            in->cursor %= MAX_OUTPUT_BUFFER_SIZE;
        }
        return paContinue;
    }

    static int inputCallback(const void *inputBuffer, void *outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo *timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void *userData) {
        const auto *in = (const float *) inputBuffer;
        auto out = static_cast<CircularBuffer *>(userData);


        for (size_t i = 0; i < framesPerBuffer; i++) {
            out->buffer[out->cursor++] = *in++;
            out->buffer[out->cursor++] = *in++;
            out->cursor %= MAX_OUTPUT_BUFFER_SIZE;
        }
        return paContinue;
    }
};


#endif //BABEL_SERVER_SOUNDHANDLER_HPP
