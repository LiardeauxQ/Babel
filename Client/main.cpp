//
// Created by alex on 9/8/19.
//

#include "AudioController.hpp"
#include "SoundStream.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  try {
    AudioController audioController;

    auto devices = audioController.getDevicesInfo();

    for (auto dev: devices) {
      std::cout << dev->name << std::endl;
    }

    SoundStream stream();
  } catch (const AudioControllerError &e) {
    std::cerr << "Cannot create controller " << e.what() << std::endl;
  }
}