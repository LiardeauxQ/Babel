//
// Created by alex on 9/17/19.
//

#include "SoundManager.hpp"
#include <iostream>

SoundManager::SoundManager(double sampleRate)
    : stream_(nullptr)
    , buffers_ {
        std::make_unique<boost::circular_buffer<float>>(1024),
        std::make_unique<std::vector<float>>(1024),
    }
{
    int error = Pa_OpenDefaultStream(&stream_,
        1,
        1,
        paFloat32,
        sampleRate,
        paFramesPerBufferUnspecified,
        nullptr,
        nullptr);

    if (error != paNoError)
        throw AudioControllerError(error);
}

void SoundManager::read(std::vector<float>& buffer)
{
    int error = Pa_ReadStream(stream_, buffer.data(), buffer.capacity());
    buffer.resize(buffer.capacity());

    if (error != paNoError)
        throw AudioControllerError(error);
}

void SoundManager::write(const std::vector<float>& data)
{
    Pa_WriteStream(stream_, data.data(), data.size());
}

int SoundManager::callback(const void* inputBuffer, void* outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData)
{
    std::cout << "Callback" << std::endl;
    auto in = (float*)inputBuffer;
    auto out = (float*)outputBuffer;
    auto shared = (SharedData*)userData;

    for (size_t i = 0; i < framesPerBuffer; i++)
        shared->toRead->push_back(*in++);

//    if (shared->toWrite->empty())
//        return paContinue;
//
//    if (shared->toWrite->size() <= framesPerBuffer) {
//        for (size_t i = 0; i < shared->toWrite->size(); i++) {
//            *out++ = shared->toWrite->back();
//            *out++ = shared->toWrite->back();
//            shared->toWrite->pop_back();
//        }
//    } else {
//        for (size_t i = 0; i < framesPerBuffer; i++) {
//            *out++ = shared->toWrite->back();
//            *out++ = shared->toWrite->back();
//            shared->toWrite->pop_back();
//        }
//    }
    return paContinue;
}
