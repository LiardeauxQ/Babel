//
// Created by Quentin Liardeaux on 10/5/19.
//

#include "ServerResponse.hpp"

std::vector<std::tuple<int, RESULT (*)(void*)>> ServerResponse::responses = {
        std::make_tuple(SERVER_PING_RESPONSE, &ServerResponse::receivePing),
        std::make_tuple(SERVER_HELLO_RESPONSE, &ServerResponse::receiveHello),
        std::make_tuple(SERVER_REGISTER_RESPONSE, &ServerResponse::receiveRegister),
        std::make_tuple(SERVER_CALL_RESPONSE, &ServerResponse::receiveCall),
        std::make_tuple(SERVER_GOODBYE_RESPONSE, &ServerResponse::receiveBye)
};

RESULT ServerResponse::receiveResponse(Message &message)
{
    for (auto response : responses) {
        if (std::get<0>(response) == message.getId()) {
            return std::get<1>(response)(message.getPayload());
        }
    }
    return RESULT::KO;
}

RESULT ServerResponse::receivePing(void *response)
{
    server_ping_response_t *srv = (server_ping_response_t*)response;
    return RESULT::OK;
}

RESULT ServerResponse::receiveHello(void *response)
{
    server_hello_response_t *srv = (server_hello_response_t*)response;

    return srv->result == RESULT::OK ? RESULT::OK : RESULT::KO;
}

RESULT ServerResponse::receiveRegister(void *response)
{
    server_register_response_t *srv = (server_register_response_t*)response;

    return srv->result == RESULT::OK ? RESULT::OK : RESULT::KO;
}

RESULT ServerResponse::receiveCall(void *response)
{
    //TODO: srv.username
    return RESULT::OK;
}

RESULT ServerResponse::receiveBye(void *response)
{
    //TODO: srv.username
    return RESULT::OK;
}
