//
// Created by alex on 9/8/19.
//

#include "AudioController.hpp"
#include "SoundStream.hpp"
#include <iostream>
#include <random>

int callBack(const void *input, void *output, unsigned long frameCount,
             const PaStreamCallbackTimeInfo *timeInfo,
             PaStreamCallbackFlags statusFlags, void *userData) {
  auto *testOutput = static_cast<float *>(output);

  for (unsigned long i = 0; i < frameCount; i++) {
    testOutput[i] = 1.0;
  }

  return paContinue;
}

int main(int argc, char *argv[]) {
  try {
    AudioController audioController;

    auto defaultDevice = audioController.getDefaultOutputDevice();

    if (!defaultDevice) {
      std::cerr << "No default device" << std::endl;
      return 1;
    }

    PaStreamParameters out;
    out.device = audioController.getDefaultOutputId();
    out.channelCount = 2;
    out.sampleFormat = paFloat32;
    out.suggestedLatency = defaultDevice->defaultLowOutputLatency;
    out.hostApiSpecificStreamInfo = nullptr;

    auto stream = audioController.createStream(&out, nullptr, callBack, nullptr,
                                               paNoFlag, "Complex stream.");

    stream.start();

    audioController.sleep(100000);

    stream.stop();
  } catch (const AudioControllerError &e) {
    std::cerr << "Cannot create controller " << e.what() << std::endl;
    return 1;
  };
  return 0;
}