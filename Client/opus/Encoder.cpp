//
// Created by Quentin Liardeaux on 9/16/19.
//

#include "Encoder.hpp"

opus::Encoder::Encoder(InputSignalFq fs, int channels, ApplicationType type) {
    int error = 0;

    this->channels = channels;
    this->enc = opus_encoder_create(fs, channels, type, &error);
    if (this->enc == nullptr)
        throw Error("Cannot init Encoder");
}

opus::Encoder::~Encoder() {
    opus_encoder_destroy(this->enc);
}

std::vector<unsigned char> opus::Encoder::encode_packet(const opus_int16 *input, int frame_size) {
    std::vector<unsigned char> output(DATA_PACKET_SIZE);
    opus_int32 error = 0;

    error = opus_encode(this->enc, input, frame_size, output.data(), DATA_PACKET_SIZE);
    if (!error)
        throw Error("Cannot encode data");
    return output;
}

std::vector<unsigned char> opus::Encoder::encode_packet(const float *input, int frame_size) {
    std::vector<unsigned char> output(DATA_PACKET_SIZE);
    opus_int32 error = 0;

    error = opus_encode_float(this->enc, input, frame_size, output.data(), DATA_PACKET_SIZE);
    if (!error)
        throw Error("Cannot encode float data");
    return output;
}
