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
    Message(int id, int payload_len, void *payload)
        : requestUnion_ {
            .req = {
                id,
                payload_len }
        }
        , payload_(payload)
        , allocated_(true)
    {
    }

    Message() = default;

    [[nodiscard]] int getId() const { return requestUnion_.req.id; }

    [[nodiscard]] int getPayloadSize() const { return requestUnion_.req.request_len; }

    [[nodiscard]] void* getPayload() const { return payload_; }

    void* getHeaderRaw() { return requestUnion_.headerRaw; }

    void setupPayload()
    {
        payload_ = malloc(requestUnion_.req.request_len);
        if (payload_)
            allocated_ = true;
        else
            throw "Allocation error.";
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
    union {
        char headerRaw[HEADER_SIZE];
        request_header_t req;
    } requestUnion_;

    void *payload_;
    bool allocated_;
};

#endif //BABEL_SERVER_MESSAGE_HPP
