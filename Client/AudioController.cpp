//
// Created by alex on 9/8/19.
//

#include "AudioController.hpp"
#include "AudioControllerError.hpp"
#include <iostream>

inline void handleError(int error)
{
    if (error != paNoError)
        throw AudioControllerError(error);
}

AudioController::AudioController()
{
    int error = Pa_Initialize();

    handleError(error);
}

AudioController::~AudioController()
{
    int error = Pa_Terminate();

    if (error != paNoError)
        std::cerr << "Error while destructing AudioController: "
                  << Pa_GetErrorText(error) << std::endl;
}

const PaDeviceInfo* AudioController::getDefaultInputDevice() const
{
    int idx = Pa_GetDefaultInputDevice();

    if (idx == paNoDevice)
        return nullptr;
    return Pa_GetDeviceInfo(idx);
}

const PaDeviceInfo* AudioController::getDefaultOutputDevice() const
{
    int idx = Pa_GetDefaultOutputDevice();

    if (idx == paNoDevice)
        return nullptr;
    return Pa_GetDeviceInfo(idx);
}

std::vector<const PaDeviceInfo*> AudioController::getDevicesInfo() const
{
    int count = Pa_GetDeviceCount();
    std::vector<const PaDeviceInfo*> devices;

    devices.reserve(count);
    for (int i = 0; i < count; ++i)
        devices.push_back(Pa_GetDeviceInfo(i));

    return devices;
}

int AudioController::getDefaultOutputId() const
{
    return Pa_GetDefaultOutputDevice();
}

int AudioController::getDefaultInputId() const
{
    return Pa_GetDefaultInputDevice();
}
std::unique_ptr<SoundManager> AudioController::createManager(PaStreamParameters* input, PaStreamParameters* output, double sampleRate) const
{
    return std::unique_ptr<SoundManager>(new SoundManager(
        input,
        output,
        sampleRate));
}
