//
// Created by alex on 9/8/19.
//

#ifndef BABEL_AUDIO_HPP
#define BABEL_AUDIO_HPP

#include "portaudio.h"
#include <exception>

class AudioController {
public:
    AudioController();
    ~AudioController();

    void chooseDevice();

private:

};

class AudioControllerError : std::exception {
public:
    AudioControllerError(int error);

    const char *what() const noexcept;

private:
    int error_;
};

#endif //BABEL_AUDIO_HPP
