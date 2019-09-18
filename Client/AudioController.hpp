//
// Created by alex on 9/8/19.
//

#ifndef BABEL_AUDIO_HPP
#define BABEL_AUDIO_HPP

#include "SoundStream.hpp"
#include "portaudio.h"
#include <exception>
#include <memory>
#include <vector>

class AudioController {
public:
    // Initialize the AudioController can fail and throw `AudioControllerError`.
    AudioController();

    ~AudioController();

    // Get number version of the program.
    static int getVersion();

    // Get text version of the program.
    static std::string getTextVersion();

    [[nodiscard]] std::vector<const PaDeviceInfo*> getDevicesInfo() const;

    SoundStream createStream(const PaDeviceInfo* dev,
        PaStreamCallback* callback,
        void* linkedData,
        const std::string& name);

    SoundStream createCustomStream(PaStreamParameters* outputStream,
        PaStreamParameters* inputStream,
        PaStreamCallback* callback, void* userData,
        PaStreamFlags flags, const std::string& name);

    [[nodiscard]] const PaDeviceInfo* getDefaultInputDevice() const;

    [[nodiscard]] const PaDeviceInfo* getDefaultOutputDevice() const;

    [[nodiscard]] int getDefaultOutputId() const;

    [[nodiscard]] int getDefaultInputId() const;

    inline void sleep(long ms) { Pa_Sleep(ms); }
};

class AudioControllerError : std::exception {
public:
    explicit AudioControllerError(int error);

    [[nodiscard]] const char* what() const noexcept final;

private:
    int error_;
};

#endif // BABEL_AUDIO_HPP
