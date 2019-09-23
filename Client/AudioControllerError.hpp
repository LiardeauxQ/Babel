//
// Created by alex on 9/20/19.
//

#ifndef BABEL_SERVER_AUDIOCONTROLLERERROR_HPP
#define BABEL_SERVER_AUDIOCONTROLLERERROR_HPP

#include <portaudio.h>
#include <exception>

class AudioControllerError : std::exception {
public:
    explicit AudioControllerError(int error);

    [[nodiscard]] const char* what() const noexcept final;

private:
    int error_;
};
#endif //BABEL_SERVER_AUDIOCONTROLLERERROR_HPP
