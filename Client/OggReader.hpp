//
// Created by alex on 9/10/19.
//

#ifndef BABEL_SERVER_OGGREADER_HPP
#define BABEL_SERVER_OGGREADER_HPP

#include "ISoundFormatReader.hpp"

class OggReader : public ISoundFormatReader {
private:
  OggReader() : internalData_(nullptr) {}

  int load(const std::string &filePath) final { return 0; }

  int play(int count) final { return 0; }

public:
  char *internalData_;
};

#endif // BABEL_SERVER_OGGREADER_HPP
