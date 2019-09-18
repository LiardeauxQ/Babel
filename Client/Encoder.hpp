//
// Created by Quentin Liardeaux on 9/16/19.
//

#ifndef BABEL_SERVER_ENCODER_HPP
#define BABEL_SERVER_ENCODER_HPP

#include <stdlib.h>

#include "Opus.hpp"
#include "Packet.hpp"
#include "Error.hpp"

namespace opus {
    class Encoder {
    public:

        Encoder(InputSignalFq fd, int channels, ApplicationType type);
        ~Encoder();

        Packet encode_packet(const opus_int16 *input, int frame_size);
        Packet encode_packet(const float *input, int frame_size);
    private:
        OpusEncoder *enc;
        int channels;
    };
}

#endif //BABEL_SERVER_ENCODER_HPP
