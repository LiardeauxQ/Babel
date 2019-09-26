//
// Created by alex on 9/8/19.
//

#include "AudioController.hpp"
#include "AudioControllerError.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    try {
        AudioController audioController;

        auto soundManager = audioController.createManager();

        soundManager->start();

        std::vector<float> data;
        data.reserve(50000);

        while (soundManager->isActive()) {
            soundManager->read(data);

            soundManager->write(data);
            data.clear();

            audioController.sleep(1000);
        }

        soundManager->stop();
    } catch (const AudioControllerError& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    };
    return 0;
}
