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
    User(std::string name, const std::string& password)
        : name_(std::move(name))
        , password_(name)
        , id_(0)
    {
    }

    // Create user from connection and username.
    User(const Database& connection, std::string username)
        : name_(std::move(username))
        , password_()
        , id_(0)
    {

    }

    // Create user from connection and id.
    User(const Database& connection, const int id)
        : name_()
        , password_()
        , id_(id)
    {
    }

    User(const User&) = delete;

    ~User() = default;

private:
    std::string name_;
    std::string password_;
    size_t id_;
};

#endif //BABEL_SERVER_USER_HPP
