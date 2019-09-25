//
// Created by alex on 9/24/19.
//

#ifndef BABEL_SERVER_MESSAGE_HPP
#define BABEL_SERVER_MESSAGE_HPP

#include "protocol.h"
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <glob.h>
#include <iostream>

static const size_t MAX_LEN = 1024 * 256;

class Message {
public:
    // Construct a message from it's id and it's payload.
    Message(int id, void* payload)
        : message_(new request_t { id, -1, nullptr })
        , ready_(false)
        , headerUnion_()
    {
        message_->request_len = getSizeFromId(id);

        if (message_->request_len == -1)
            throw "Unknown message type.";
    };

    Message() = default;

    char *header()
    {
        return (char *)message_;
    }

    void *payload()
    {
        return message_->payload;
    }

    int setupPayload()
    {
        int size = getSizeFromId(message_->id);

        message_->payload = malloc(size);
        return size;
    }

    int getId()
    {
        return message_->id;
    }

    static int getSizeFromId(int id)
    {
        switch (id) {
        case CLIENT_HELLO:
            return CLIENT_HELLO_SIZE;
        case CLIENT_PING:
            return CLIENT_PING_SIZE;
        case CLIENT_ACCEPT_FRIEND:
            return CLIENT_ACCEPT_FRIEND_SIZE;
        case CLIENT_FRIEND_REQUEST:
            return CLIENT_FRIEND_REQUEST_SIZE;
        case CLIENT_BYE:
            return CLIENT_BYE_SIZE;
        case CLIENT_CALL:
            return CLIENT_CALL_SIZE;
        case CLIENT_GOODBYE:
            return CLIENT_GOODBYE_SIZE;
        case CLIENT_FRIEND_STATUS:
            return CLIENT_FRIEND_STATUS_SIZE;
        case SERVER_ACCEPT_FRIEND:
            return SERVER_ACCEPT_FRIEND_SIZE;
        case SERVER_BYE:
            return SERVER_BYE_SIZE;
        case SERVER_CALL:
            return SERVER_CALL_SIZE;
        case SERVER_FRIEND_REQUEST:
            return SERVER_FRIEND_REQUEST_SIZE;
        default:
            return -1;
        }
    }

private:
    request_t* message_;

    union {
        char headerRaw[8];
        struct {
            int id;
            int size;
        } headerInt;
    } headerUnion_;

    bool ready_;
};

#endif //BABEL_SERVER_MESSAGE_HPP
