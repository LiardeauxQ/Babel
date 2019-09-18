//
// Created by alex on 9/8/19.
//

#include "AudioController.hpp"
#include "SoundManager.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    try {
        AudioController audioController;
        SoundManager soundManager(audioController);

        soundManager.start();

        while (true) {
            auto data = soundManager.read();

            if (data)
                soundManager.write(std::make_unique<std::vector<float>>(data->begin(), data->end()));
        }

        soundManager.stop();
    } catch (const AudioControllerError& e) {
        std::cerr << "Cannot create controller " << e.what() << std::endl;
        return 1;
    };
    return 0;
}
