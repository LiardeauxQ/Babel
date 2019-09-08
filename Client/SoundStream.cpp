//
// Created by alex on 9/8/19.
//

#include "SoundStream.hpp"

#include "AudioController.hpp"
#include <utility>

int callBack(const void *input, void *output, unsigned long frameCount,
             const PaStreamCallbackTimeInfo *timeInfo,
             PaStreamCallbackFlags statusFlags, void *userData) {
  std::cout << "Callback" << std::endl;
  return 1;
}

SoundStream::SoundStream(double sampleRate, int numInputChannels,
                         int numOutputChannels, PaSampleFormat sampleFormat,
                         unsigned long framesPerBuffer,
                         PaStreamCallback *streamCallback, void *linkedData,
                         std::string name)
    : stream_(nullptr), name_(std::move(name)) {
  int error = Pa_OpenDefaultStream(
      &stream_, numInputChannels, numOutputChannels, paFloat32, sampleRate,
      paFramesPerBufferUnspecified, streamCallback, linkedData);

  if (error != paNoError)
    throw AudioControllerError(error);
}

SoundStream::~SoundStream() { Pa_CloseStream(stream_); }
