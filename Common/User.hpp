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
    User(const std::string& username, const std::string& password, int id)
        : username(username)
        , password(password)
    {}

    ~User() = default;

    std::string username;
    std::string password;
    int id;
private:
    friend std::ostream& operator<<(std::ostream& os, const User& user);
};

#endif //BABEL_SERVER_USER_HPP
