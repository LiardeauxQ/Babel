//
// Created by alex on 9/20/19.
//

#include "AudioControllerError.hpp"
#include "AudioController.hpp"
#include <iostream>
const char* AudioControllerError::what() const noexcept
{
    return Pa_GetErrorText(error_);
}
AudioControllerError::AudioControllerError(int error)
    : error_(error)
{
}