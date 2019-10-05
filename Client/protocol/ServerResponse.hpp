//
// Created by Quentin Liardeaux on 10/5/19.
//

#ifndef BABEL_SERVER_SERVERRESPONSE_HPP
#define BABEL_SERVER_SERVERRESPONSE_HPP

#include <vector>
#include <tuple>

#include "protocol.h"
#include "Message.hpp"

class ServerResponse {
public:
    static RESULT receiveResponse(Message &message);

protected:
    static RESULT receivePing(void *response);
    static RESULT receiveHello(void *response);
    static RESULT receiveRegister(void *response);
    static RESULT receiveCall(void *response);
    static RESULT receiveBye(void *response);

    static std::vector<std::tuple<int, RESULT (*)(void*)>> responses;
};

#endif //BABEL_SERVER_SERVERRESPONSE_HPP
