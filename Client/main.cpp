//
// Created by alex on 9/8/19.
//

#include <iostream>
#include "AudioController.hpp"

int main(int argc, char *argv[]) {
    try {
        AudioController audioController();
    } catch(const AudioControllerError &e) {
        std::cerr << "Cannot create controller " << e.what() << std::endl;
    }
}
