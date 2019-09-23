//
// Created by alex on 9/23/19.
//

#ifndef BABEL_SERVER_USER_HPP
#define BABEL_SERVER_USER_HPP

#include <string>

class User {
public:
    User(const std::string& name, const std::string& password)
        : name(name)
        , password(name)
        , idDatabase(0)
    {
    }

    User(const User&) = delete;

    ~User() = default;

private:
    std::string name;
    std::string password;
    size_t idDatabase;
};

#endif //BABEL_SERVER_USER_HPP
