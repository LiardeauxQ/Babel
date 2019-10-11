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

const size_t MAX_SIZE = 32000;

class SoundManager {
public:
    typedef float(*middlewareFunc)(float data);
    // Should be used.
    SoundManager(PaStreamParameters* input, PaStreamParameters *output, double sampleRate, middlewareFunc middleware = nullptr);

    // Discard internal data and give it to user.
    void read(std::vector<float>& buffer);

    // Raw read
    size_t read(float *buffer, size_t n);

    // Append to internal buffer the new data.
    void write(const std::vector<float>& data);

    // Append to internal buffer the new data.
    void write(float *buffer, size_t n);

    // Enable streaming interaction.
    inline void start() { Pa_StartStream(stream_); }

    // Enable streaming interaction.
    inline void stop() { Pa_StopStream(stream_); }

    inline bool isActive() { return Pa_IsStreamActive(stream_); }

    inline void setMiddleware(middlewareFunc func) { buffers_.middleware = func; }

    [[nodiscard]] double getTime() const { return Pa_GetStreamTime(stream_); }

private:
    PaStream* stream_;

    struct SharedData {
        std::unique_ptr<boost::circular_buffer<float>> toRead;
        std::unique_ptr<boost::circular_buffer<float>> toWrite;
        middlewareFunc middleware;
    } buffers_;

    static int callback(const void* inputBuffer, void* outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void* userData);
};

#endif //BABEL_SERVER_SOUNDMANAGER_HPP
