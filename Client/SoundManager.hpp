//
// Created by alex on 9/17/19.
//

#ifndef BABEL_SERVER_SOUNDMANAGER_HPP
#define BABEL_SERVER_SOUNDMANAGER_HPP

#include "AudioControllerError.hpp"
#include <boost/circular_buffer.hpp>
#include <iostream>
#include <memory>
#include <portaudio.h>
#include <vector>

const size_t MAX_SIZE = 2048;
const size_t DEFAULT_SAMPLE_RATE = 44100;
const size_t DEFAULT_FRAME_SIZE = 64;

class SoundManager {
public:
    // Should be used.
    SoundManager(PaStreamParameters* input, PaStreamParameters* output, double sampleRate = DEFAULT_SAMPLE_RATE, unsigned long framePerBuffer = paFramesPerBufferUnspecified)
        : outputStream_(nullptr)
        , inputStream_(nullptr)
        , nbInChannels_(input->channelCount)
        , nbOutChannels_(output->channelCount)
        , buffers_ {
            std::make_unique<boost::circular_buffer<float>>(MAX_SIZE),
            std::make_unique<std::vector<float>>(MAX_SIZE),
            nbInChannels_,
            nbOutChannels_
        }
    {
        int error = Pa_OpenStream(&inputStream_, input, nullptr, sampleRate, framePerBuffer, paNoFlag, inputCallback, (void*)&buffers_);

        if (error != paNoError)
            throw AudioControllerError(error);

        error = Pa_OpenStream(&outputStream_, nullptr, output, sampleRate, framePerBuffer, paNoFlag, outputCallback, (void*)&buffers_);

        if (error != paNoError)
            throw AudioControllerError(error);
    }

    // Discard internal data and give it to user.
    [[nodiscard]] std::unique_ptr<boost::circular_buffer<float>> read()
    {
        if (buffers_.inputBuffer->full()) {
            auto newVec = std::make_unique<boost::circular_buffer<float>>(MAX_SIZE);
            buffers_.inputBuffer.swap(newVec);
            return newVec;
        }
        return nullptr;
    }

    // Append to internal buffer the new data.
    void write(const std::vector<float>& data)
    {
        if (buffers_.outputBuffer->size() < MAX_SIZE)
            buffers_.outputBuffer->insert(buffers_.outputBuffer->begin(), data.begin(), data.end());
    }

    // Enable streaming interaction.
    inline void start()
    {
        Pa_StartStream(outputStream_);
        Pa_StartStream(inputStream_);
    }

    // Enable streaming interaction.
    inline void stop()
    {
        Pa_StopStream(outputStream_);
        Pa_StopStream(inputStream_);
    }

private:
    typedef std::unique_ptr<boost::circular_buffer<float>> UniqueCircular;
    typedef std::unique_ptr<std::vector<float>> UniqueBuffer;

    PaStream* inputStream_;
    PaStream* outputStream_;

    int nbInChannels_;
    int nbOutChannels_;

    struct SharedData {
        UniqueCircular inputBuffer;
        UniqueBuffer outputBuffer;
        int& nbInChannel;
        int& nbOutChannel;
    } buffers_;

    static int outputCallback(const void* inputBuffer, void* outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void* userData);

    static int inputCallback(const void* inputBuffer, void* outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void* userData);
};

#endif //BABEL_SERVER_SOUNDMANAGER_HPP
