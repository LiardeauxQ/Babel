//
// Created by Quentin Liardeaux on 9/16/19.
//

#ifndef BABEL_SERVER_ENCODER_HPP
#define BABEL_SERVER_ENCODER_HPP

#include "opus.h"
#include <stdlib.h>

#include "Packet.hpp"
#include "Error.hpp"

class Encoder {
public:

    enum InputSignalFrequency: opus_int32 {
        veryLow = 8000,
        low = 12000,
        meduim = 16000,
        high = 24000,
        veryHigh = 48000
    };

    enum ApplicationType {
        voip = OPUS_APPLICATION_VOIP,
        audio = OPUS_APPLICATION_AUDIO,
        restrictedLowdelay = OPUS_APPLICATION_RESTRICTED_LOWDELAY
    };

    Encoder(InputSignalFrequency fd, int channels, ApplicationType type);
    ~Encoder();

    Packet encode_packet(const opus_int16 *input, int frame_size);
    Packet encode_packet(const float *input, int frame_size);
private:
    OpusEncoder *enc;
    int channels;
};

#endif //BABEL_SERVER_ENCODER_HPP
