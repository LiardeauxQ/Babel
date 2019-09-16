//
// Created by alex on 9/8/19.
//

#include "AudioController.hpp"
#include "SoundStream.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>

struct UserData {
  char *data;
  unsigned long offset;
};

int callBack(const void *input, void *output, unsigned long frameCount,
             const PaStreamCallbackTimeInfo *timeInfo,
             PaStreamCallbackFlags statusFlags, void *userData) {
  return paComplete;
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

    while (stream.isActive()) {
      audioController.sleep(1000);
    }

    stream.stop();
  } catch (const AudioControllerError &e) {
    std::cerr << "Cannot create controller " << e.what() << std::endl;
    return 1;
  };
  return 0;
}
