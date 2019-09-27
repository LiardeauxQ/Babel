//
// Created by Quentin Liardeaux on 9/18/19.
//

#ifndef BABEL_SERVER_PACKET_HPP
#define BABEL_SERVER_PACKET_HPP

#define DATA_PACKET_SIZE 1000

struct Packet {
    unsigned char data[DATA_PACKET_SIZE];
};

#endif //BABEL_SERVER_PACKET_HPP
