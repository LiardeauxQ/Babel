//
// Created by alex on 9/8/19.
//

#include "AudioController.hpp"
#include <iostream>

#define SAMPLE_RATE (44100)     // TODO Add this to configuration.
#define FRAME_PER_BUFFER (1024) // TODO Add this to configuration.

inline void handleError(int error) {
    if (error != paNoError)
        throw AudioControllerError(error);
}

AudioController::AudioController() {
    int error = Pa_Initialize();

    handleError(error);
}

AudioController::~AudioController() {
    int error = Pa_Terminate();

    if (error != paNoError)
        std::cerr << "Error while destructing AudioController: "
                  << Pa_GetErrorText(error) << std::endl;
}

const PaDeviceInfo *AudioController::getDefaultInputDevice() const {
    int idx = Pa_GetDefaultInputDevice();

    if (idx == paNoDevice)
        return nullptr;
    return Pa_GetDeviceInfo(idx);
}

const PaDeviceInfo *AudioController::getDefaultOutputDevice() const {
    int idx = Pa_GetDefaultOutputDevice();

    if (idx == paNoDevice)
        return nullptr;
    return Pa_GetDeviceInfo(idx);
}

int AudioController::getVersion() { return Pa_GetVersion(); }

std::string AudioController::getTextVersion() {
    return std::string(Pa_GetVersionText());
}

std::vector<const PaDeviceInfo *> AudioController::getDevicesInfo() const {
    int count = Pa_GetDeviceCount();
    std::vector<const PaDeviceInfo *> devices;

    devices.reserve(count);
    for (int i = 0; i < count; ++i)
        devices.push_back(Pa_GetDeviceInfo(i));

    return devices;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "MemberFunctionCanBeStatic"

SoundStream AudioController::createStream(
        const PaDeviceInfo *dev, PaStreamCallback *callback, void *linkedData,
        const std::string &name) {
    return SoundStream(dev->defaultSampleRate, dev->maxInputChannels,
                       dev->maxOutputChannels, paFloat32,
                       64, callback, linkedData,
                       name); // TODO Change framePerBuffer
}

SoundStream AudioController::createCustomStream(PaStreamParameters *outputStream, PaStreamParameters *inputStream,
                                                PaStreamCallback *callback, void *userData, PaStreamFlags flags,
                                                const std::string &name) {
    return SoundStream(outputStream, inputStream, SAMPLE_RATE,
                       64, flags, callback, userData,
                       name);
}

int AudioController::getDefaultOutputId() const {
    return Pa_GetDefaultOutputDevice();
}

int AudioController::getDefaultInputId() const {
    return Pa_GetDefaultInputDevice();
}

#pragma clang diagnostic pop

const char *AudioControllerError::what() const noexcept {
    return Pa_GetErrorText(error_);
}

AudioControllerError::AudioControllerError(int error) : error_(error) {}
