//
// Created by alex on 9/24/19.
//

#ifndef BABEL_SERVER_MESSAGE_HPP
#define BABEL_SERVER_MESSAGE_HPP

#include "protocol.h"
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>

static const size_t MAX_LEN = 1024 * 256;

class Message {
public:
    // Construct a message from it's id and it's payload.
    Message(int id, int payload_len, void *payload)
        : requestUnion_ {}
        , payload_(payload)
        , data_(nullptr)
        , allocated_(true)
    {
        requestUnion_.req.id = id;
        requestUnion_.req.request_len = payload_len;
    }

    ~Message()
    {
        free(data_);
    }

    Message()
        : requestUnion_ {}
        , payload_(nullptr)
        , data_(nullptr)
        , allocated_(false)
    {
    }

    [[nodiscard]] int getId() const { return requestUnion_.req.id; }

    [[nodiscard]] int getPayloadSize() const { return requestUnion_.req.request_len; }

    [[nodiscard]] size_t getTotalSize() const { return requestUnion_.req.request_len + HEADER_SIZE; }

    [[nodiscard]] void* getPayload() const { return payload_; }

    void* getHeaderRaw() { return requestUnion_.headerRaw; }

    void* getData()
    {
        if (data_ == nullptr) {
            data_ = malloc(getTotalSize());
        }
        memcpy(data_, requestUnion_.headerRaw, HEADER_SIZE);
        memcpy(((unsigned char*)data_) + HEADER_SIZE, payload_, getPayloadSize());
        return data_;
    }

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
    void *data_;
    bool allocated_;
};

#endif //BABEL_SERVER_MESSAGE_HPP
