//
// Created by alex on 9/10/19.
//

#ifndef BABEL_SERVER_SOUNDFORMATREADER_HPP
#define BABEL_SERVER_SOUNDFORMATREADER_HPP

#include <string>

class ISoundFormatReader {
  virtual int load(const std::string &filePath) = 0;
  virtual int play(int count) = 0;
};

#endif // BABEL_SERVER_SOUNDFORMATREADER_HPP
