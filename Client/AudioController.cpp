//
// Created by alex on 9/8/19.
//

#include "AudioController.hpp"
#include <iostream>

#define SAMPLE_RATE (44100) // TODO Add this to configuration.

inline void handleError(int error) {
  if (error != paNoError)
    throw AudioControllerError(error);
}

AudioController::AudioController() {
  int error = Pa_Initialize();

  handleError(error);


  handleError(error);
}

AudioController::~AudioController() {
  int error = Pa_Terminate();

  if (error != paNoError)
    goto error;

  return;

error:
  std::cerr << "Error while destructing AudioController: "
            << Pa_GetErrorText(error) << std::endl;
}

void AudioController::chooseDevice() {
  std::cout << "Changing device." << std::endl;
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

const char *AudioControllerError::what() const noexcept {
  return Pa_GetErrorText(error_);
}

AudioControllerError::AudioControllerError(int error) : error_(error) {}