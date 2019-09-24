//
// Created by alex on 9/17/19.
//

#include "SoundManager.hpp"
#include <iostream>

int SoundManager::outputCallback(const void* inputBuffer, void* outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData)
{
    auto out = (float*)outputBuffer;
    auto data = (SharedData*)userData;

    if (data->outputBuffer->empty())
        return paContinue;
    if (data->outputBuffer->size() <= framesPerBuffer * data->nbOutChannel) {
        for (size_t i = 0; i < data->outputBuffer->size(); i += data->nbOutChannel) {
            *out++ = data->outputBuffer->back();
            data->outputBuffer->pop_back();
            *out++ = data->outputBuffer->back();
            data->outputBuffer->pop_back();
        }
    } else {
        for (size_t i = 0; i < framesPerBuffer; i++) {
            *out++ = data->outputBuffer->back();
            data->outputBuffer->pop_back();
            *out++ = data->outputBuffer->back();
            data->outputBuffer->pop_back();
        }
    }
    return paContinue;
}

int SoundManager::inputCallback(const void* inputBuffer, void* outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData)
{
    auto data = static_cast<SharedData*>(userData);
    const auto* micro = static_cast<const float*>(inputBuffer);

    for (size_t i = 0; i < framesPerBuffer; i++)
        for (size_t i = 0; i < data->nbInChannel; i++)
            data->inputBuffer->push_back(*micro++); // push left.

    return paContinue;
}
