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
    User(Database& connection, const std::string& username);

    // Create user from connection and id.
    User(Database& connection, int id);

    User(const User&) = delete;

    ~User() = default;

private:
    static int takeUser(void* toChange, int argc, char** argv, char** azColName);

    struct callbackData {
        std::string& name;
        std::string& password;
        size_t& id;
        bool used;
    };

    std::string name_;
    std::string password_;
    size_t id_;

    friend std::ostream& operator<<(std::ostream& os, const User& user);
};

#endif //BABEL_SERVER_USER_HPP
