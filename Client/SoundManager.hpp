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

const size_t MAX_SIZE = 50000;

class SoundManager {
public:
    // Should be used.
    explicit SoundManager(PaStreamParameters* input, PaStreamParameters *output, double sampleRate);

    // Discard internal data and give it to user.
    void read(std::vector<float>& buffer);

    // Append to internal buffer the new data.
    void write(const std::vector<float>& data);

    // Enable streaming interaction.
    inline void start() { Pa_StartStream(stream_); }

    // Enable streaming interaction.
    inline void stop() { Pa_StopStream(stream_); }

    inline bool isActive() { return Pa_IsStreamActive(stream_); }

private:
    PaStream* stream_;

    struct SharedData {
        std::unique_ptr<boost::circular_buffer<float>> toRead;
        std::unique_ptr<std::vector<float>> toWrite;
    } buffers_;

    static int callback(const void* inputBuffer, void* outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void* userData);
};

#endif //BABEL_SERVER_SOUNDMANAGER_HPP
