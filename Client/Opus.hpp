//
// Created by Quentin Liardeaux on 9/18/19.
//

#ifndef BABEL_SERVER_OPUS_HPP
#define BABEL_SERVER_OPUS_HPP

#include "opus.h"

namespace opus {
    enum InputSignalFq : opus_int32 {
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
}

#endif //BABEL_SERVER_OPUS_HPP
