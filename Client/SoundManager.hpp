//
// Created by alex on 9/17/19.
//

#ifndef BABEL_SERVER_SOUNDMANAGER_HPP
#define BABEL_SERVER_SOUNDMANAGER_HPP

#include "AudioController.hpp"
#include "SoundStream.hpp"
#include <boost/circular_buffer.hpp>
#include <memory>
#include <vector>

const size_t MAX_SIZE = 64000;

class SoundManager {
public:
    explicit SoundManager(AudioController& controller)
        : buffers_ {
            std::make_unique<boost::circular_buffer<float>>(MAX_SIZE),
            std::make_unique<std::vector<float>>(MAX_SIZE),
        }
        , stream_ { createSoundStream(controller, (void*)&buffers_) }
    {
    }

    [[nodiscard]] std::unique_ptr<boost::circular_buffer<float>> read()
    {
        if (buffers_.inputBuffer->size() > 0) {
            auto newVec = std::make_unique<boost::circular_buffer<float>>(MAX_SIZE);
            buffers_.inputBuffer.swap(newVec);
            return newVec;
        }
        return nullptr;
    }

    void write(const std::unique_ptr<std::vector<float>> data)
    {
        buffers_.outputBuffer->reserve(buffers_.outputBuffer->size() + data->size());
        buffers_.outputBuffer->insert(buffers_.outputBuffer->end(), data->begin(), data->end());
    }

    // Enable streaming interaction.
    inline void start()
    {
        stream_.start();
    }

    // Enable streaming interaction.
    inline void stop()
    {
        stream_.stop();
    }

private:
    static SoundStream createSoundStream(AudioController& controller, void* data)
    {
        auto in = PaStreamParameters {
            .device = controller.getDefaultInputId(),
            .channelCount = 2,
            .sampleFormat = paFloat32,
            .suggestedLatency = controller.getDefaultInputDevice()->defaultLowInputLatency,
            .hostApiSpecificStreamInfo = nullptr,
        };

        auto out = PaStreamParameters {
            .device = controller.getDefaultOutputId(),
            .channelCount = 2,
            .sampleFormat = paFloat32,
            .suggestedLatency = controller.getDefaultOutputDevice()->defaultLowOutputLatency,
            .hostApiSpecificStreamInfo = nullptr,
        };

        return controller.createCustomStream(
            &out,
            &in,
            callback,
            data,
            paNoFlag,
            "CustomStream");
    }

    static int callback(const void* inputBuffer, void* outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void* userData)
    {
        auto out = static_cast<float*>(outputBuffer);
        auto data = static_cast<Buffers*>(userData);
        const auto* micro = static_cast<const float*>(inputBuffer);

        for (size_t i = 0; i < framesPerBuffer; i++) {
            data->inputBuffer->push_back(*micro++); // push left.
            data->inputBuffer->push_back(*micro++); // push right.
        }

        for (size_t i = 0; i < framesPerBuffer; ++i) {
            if (data->outputBuffer->size() >= framesPerBuffer * 2) {
                *out++ = data->outputBuffer->back();
                data->outputBuffer->pop_back();
                *out++ = data->outputBuffer->back();
                data->outputBuffer->pop_back();
            } else {
                return paContinue;
            }
        }

        return paContinue;
    }

    struct Buffers {
        std::unique_ptr<boost::circular_buffer<float>> inputBuffer;
        std::unique_ptr<std::vector<float>> outputBuffer;
    } buffers_;

    SoundStream stream_;
};

#endif //BABEL_SERVER_SOUNDMANAGER_HPP
