//
// Created by alex on 9/17/19.
//

#include "SoundManager.hpp"
#include <iostream>

SoundManager::SoundManager(PaStreamParameters* input, PaStreamParameters* output, double sampleRate, middlewareFunc middleware)
    : stream_(nullptr)
    , buffers_ {
        std::make_unique<boost::circular_buffer<float>>(MAX_SIZE),
        std::make_unique<boost::circular_buffer<float>>(MAX_SIZE),
        middleware
    }
{
    int error = Pa_OpenStream(
        &stream_,
        input,
        output,
        sampleRate,
        256,
        paClipOff,
        callback,
        &buffers_);

    if (error != paNoError)
        throw AudioControllerError(error);
}

// Copy all information to read buffer and discard local data.
void SoundManager::read(std::vector<float>& buffer)
{
    for (auto e : *buffers_.toRead)
        buffer.push_back(e);
    buffers_.toRead->clear();
    buffers_.toRead->resize(0);
}

// Copy all information to read buffer and discard local data.
size_t SoundManager::read(float* buffer, size_t n)
{
    size_t size = buffers_.toRead->size();
    size_t i;

    for (i = 0; i < n && i < size; ++i)
        buffer[i] = (*buffers_.toRead)[i];
    buffers_.toRead->rotate(buffers_.toRead->begin() + i);
    buffers_.toRead->resize(buffers_.toRead->size() - i);
    return i;
}

void SoundManager::write(const std::vector<float>& data)
{
    buffers_.toWrite->insert(buffers_.toWrite->begin(), data.begin(), data.end());
}

void SoundManager::write(float* buffer, size_t n)
{
    buffers_.toWrite->insert(buffers_.toWrite->begin(), buffer, buffer + n);
}

int SoundManager::callback(const void* inputBuffer, void* outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData)
{
    auto in = (float*)inputBuffer;
    auto out = (float*)outputBuffer;
    auto shared = (SharedData*)userData;

    for (size_t i = 0; i < framesPerBuffer; i++) {
        shared->toRead->push_front(*in++);

        if (shared->toWrite->empty()) {
            *out++ = 0;
            *out++ = 0;
        } else {
            auto toOutput = shared->toWrite->back();

            if (shared->middleware)
                shared->middleware(toOutput);

            *out++ = toOutput;
            *out++ = toOutput;
            shared->toWrite->pop_back();
        }
    }
    return paContinue;
}
