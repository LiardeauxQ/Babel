//
// Created by Quentin Liardeaux on 9/27/19.
//

#ifndef BABEL_SERVER_SERVERREQUEST_HPP
#define BABEL_SERVER_SERVERREQUEST_HPP

#include <string>
#include <vector>
#include <tuple>
#include <time.h>

#include "Message.hpp"
#include "protocol.h"

class ServerRequest {
public:
    static Message createRequest(int id, std::map<std::string, void*> userInfo);

    static Message createPingRequest(std::map<std::string, void*> userInfo);
    static Message createHelloRequest(std::map<std::string, void*> userInfo);
    static Message createGoodbyeRequest(std::map<std::string, void*> userInfo);
    static Message createFriendStatusRequest(std::map<std::string, void*> userInfo);
    static Message createRegisterRequest(std::map<std::string, void*> userInfo);
    static Message createFriendRequest(std::map<std::string, void*> userInfo);
    static Message createCallRequest(std::map<std::string, void*> userInfo);
    static Message createByeRequest(std::map<std::string, void*> userInfo);
    static Message createAcceptFriendRequest(std::map<std::string, void*> userInfo);
};


#endif //BABEL_SERVER_SERVERREQUEST_HPP
