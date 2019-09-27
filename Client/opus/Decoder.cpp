//
// Created by Quentin Liardeaux on 9/18/19.
//

#include "Decoder.hpp"

opus::Decoder::Decoder(InputSignalFq fs, int channels) {
    int error = 0;

    this->channels = channels;
    this->dec = opus_decoder_create(fs, channels, &error);
    if (this->dec == nullptr)
        throw Error("Cannot init Decoder");
}

opus::Decoder::~Decoder() {
    opus_decoder_destroy(this->dec);
}

std::vector<opus_int16> opus::Decoder::decode_to_bytes(const unsigned char *input, int frame_size) {
    std::vector<opus_int16> output(DATA_PACKET_SIZE);
    opus_int32 error = 0;

    error = opus_decode(this->dec, input, DATA_PACKET_SIZE, output.data(),
            frame_size, 0);
    if (!error)
        throw Error("Cannot encode data");
    return output;
}

std::vector<float> opus::Decoder::decode_to_floats(const unsigned char *input, int frame_size) {
    std::vector<float> output(DATA_PACKET_SIZE);
    opus_int32 error = 0;

    error = opus_decode_float(this->dec, input, DATA_PACKET_SIZE,
            output.data(), frame_size, 0);
    if (!error)
        throw Error("Cannot encode float data");
    return output;
}