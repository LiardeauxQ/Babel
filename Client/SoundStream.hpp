//
// Created by alex on 9/8/19.
//

#ifndef BABEL_CLIENT_SOUNDSTREAM_HPP
#define BABEL_CLIENT_SOUNDSTREAM_HPP

#include "portaudio.h"
#include <iostream>
#include <string>

class SoundStream {
public:
  SoundStream(double sampleRate, int numInputChannels, int numOutputChannels,
              PaSampleFormat sampleFormat, unsigned long framesPerBuffer,
              PaStreamCallback *streamCallback, void *linkedData,
              std::string name);

  ~SoundStream();

private:
  PaStream *stream_;
  std::string name_;
};

#endif // BABEL_CLIENT_SOUNDSTREAM_HPP
