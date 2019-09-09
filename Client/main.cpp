//
// Created by alex on 9/8/19.
//

#include "AudioController.hpp"
#include "SoundStream.hpp"
#include <iostream>

int callBack(const void *input, void *output, unsigned long frameCount,
             const PaStreamCallbackTimeInfo *timeInfo,
             PaStreamCallbackFlags statusFlags, void *userData) {
  const auto *testInput = static_cast<const float *>(input);
  auto *testOutput = static_cast<float *>(output);

  std::cout << "Actual time: " << timeInfo->currentTime << std::endl;

  for (unsigned long i = 0; i < frameCount; i += 2) {
    testOutput[i] = 1.0;
    testOutput[i + 1] = -1.0;
  }

  return paContinue;
}

int main(int argc, char *argv[]) {
  try {
    AudioController audioController;

    auto defaultDevice = audioController.getDefaultOutputDevice();

    std::cout << "Name: " << defaultDevice->name << std::endl;

    auto stream = audioController.createStreamFromDevice(
        defaultDevice, callBack, nullptr, "Default input stream.");

    stream.start();

    audioController.sleep(100000);

    stream.stop();
  } catch (const AudioControllerError &e) {
    std::cerr << "Cannot create controller " << e.what() << std::endl;
  };
}