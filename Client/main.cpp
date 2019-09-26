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

        auto soundManager = audioController.createManager(44100);

        soundManager->start();

        std::vector<float> data;
        data.reserve(12000);

        for (;;) {
            soundManager->read(data);

            if (!data.empty())
                soundManager->write(data);
        }

        soundManager->stop();
    } catch (const AudioControllerError& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    };
    return 0;
}
