//
// Created by Quentin Liardeaux on 9/18/19.
//

#ifndef BABEL_SERVER_DECODER_HPP
#define BABEL_SERVER_DECODER_HPP

#include "Packet.hpp"
#include "Opus.hpp"
#include "Error.hpp"

namespace opus {
    class Decoder {
    public:
        Decoder(InputSignalFq fs, int channels);

        ~Decoder();

        opus_int16 *decode_to_bytes(const unsigned char *input, int frame_size);
        float *decode_to_floats(const unsigned char *input, int frame_size);
    private:
        OpusDecoder *dec;
        int channels;
    };
}

#endif //BABEL_SERVER_DECODER_HPP
