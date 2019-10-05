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

protected:
    static Message ping(std::map<std::string, void*> userInfo);
    static Message hello(std::map<std::string, void*> userInfo);
    static Message goodbye(std::map<std::string, void*> userInfo);
    static Message friendStatus(std::map<std::string, void*> userInfo);
    static Message registerRequest(std::map<std::string, void*> userInfo);
    static Message friendRequest(std::map<std::string, void*> userInfo);
    static Message call(std::map<std::string, void*> userInfo);
    static Message bye(std::map<std::string, void*> userInfo);
    static Message acceptFriend(std::map<std::string, void*> userInfo);

    static std::vector<std::tuple<int, Message (*)(std::map<std::string, void*>)>> requests;
};


#endif //BABEL_SERVER_SERVERREQUEST_HPP
