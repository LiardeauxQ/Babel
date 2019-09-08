//
// Created by alex on 9/8/19.
//

#include "AudioController.hpp"

AudioController::AudioController() {
    int error = Pa_Initialize();

    if (error != 0) {
        throw AudioControllerError(error);
    }
}

const char *AudioControllerError::what() const noexcept {
    switch (error_) {
        case paNotInitialized:
            return "Not initialized.";
        case paUnanticipatedHostError:
            return "Host error.";
        case paInvalidChannelCount:
            return "Invalid channel count.";
        case paInvalidSampleRate:
            return "Invalid sample rate.";
        case paInvalidDevice:
            return "Invalid device.";
        case paInvalidFlag:
            return "Invalid flag.";
        case paSampleFormatNotSupported:
            return "Format not supported.";
        case paBadIODeviceCombination:
            return "Bad io device combination.";
        case paInsufficientMemory:
            return "Insufficient memory.";
        case paBufferTooBig:
            return "Too big buffer";
        case paBufferTooSmall:
            return "Too small buffer";
        case paNullCallback:
            return "Null callback.";
        case paBadStreamPtr:
            return "Bad stream.";
        case paTimedOut:
            return "Timed out.";
        case paInternalError:
            return "Internal error.";
        case paDeviceUnavailable:
            return "Device unavailable.";
        case paIncompatibleHostApiSpecificStreamInfo:
            return "Incompatible host api specific stream info.";
        case paStreamIsStopped:
            return "Stream is stopped.";
        case paStreamIsNotStopped:
            return "Stream is not stopped.";
        case paInputOverflowed:
            return "Input overflowed.";
        case paOutputUnderflowed:
            return "Output under flowed.";
        case paHostApiNotFound:
            return "Host api not found.";
        case paInvalidHostApi:
            return "Invalid host api.";
        case paCanNotReadFromACallbackStream:
            return "Can not read from a callback stream";
        case paCanNotWriteToACallbackStream:
            return "Can not write to a callback stream.";
        case paCanNotReadFromAnOutputOnlyStream:
            return "Can not read from an output only stream.";
        case paCanNotWriteToAnInputOnlyStream:
            return "Can not write to an input only stream.";
        case paIncompatibleStreamHostApi:
            return "Incompatible stream host api.";
        case paBadBufferPtr:
            return "Bad buffer pointer.";
    };
    return "No error";
}

AudioControllerError::AudioControllerError(int error)
        : error_(error) {}