//
// Created by Quentin Liardeaux on 10/5/19.
//

#ifndef BABEL_SERVER_PACKET_HPP
#define BABEL_SERVER_PACKET_HPP

#include "protocol.h"

template <typename T>
struct Packet {
    request_header_t header;
    T payload;
}__attribute__((unpacked));

#endif //BABEL_SERVER_PACKET_HPP
