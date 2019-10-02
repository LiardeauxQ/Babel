//
// Created by alex on 9/23/19.
//

#ifndef BABEL_SERVER_USER_HPP
#define BABEL_SERVER_USER_HPP

#include "Database.hpp"
#include <string>
#include <utility>

class User {
public:
    // Create user from connection and username.
    User(const std::string& username, const std::string& password)
        : username(username)
        , password(password)
        , id(-1)
    {}

    ~User() = default;

    std::string username;
    std::string password;
    int id;

private:
    static int takeUser(void* toChange, int argc, char** argv, char** azColName);

    struct callbackData {
        std::string& name;
        std::string& password;
        size_t& id;
        bool used;
    };

    friend std::ostream& operator<<(std::ostream& os, const User& user);
};

#endif //BABEL_SERVER_USER_HPP
