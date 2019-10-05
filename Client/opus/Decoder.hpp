//
// Created by Quentin Liardeaux on 9/18/19.
//

#ifndef BABEL_SERVER_DECODER_HPP
#define BABEL_SERVER_DECODER_HPP

#include <vector>

#include "Opus.hpp"
#include "../Error.hpp"

#define DATA_PACKET_SIZE 0

namespace opus {
    class Decoder {
    public:
        Decoder(InputSignalFq fs, int channels);

        ~Decoder();

        std::vector<opus_int16> decode_to_bytes(const unsigned char *input, int frame_size);
        std::vector<float> decode_to_floats(const unsigned char *input, int frame_size);
    private:
        OpusDecoder *dec;
        int channels;
    };
}

#endif //BABEL_SERVER_DECODER_HPP
