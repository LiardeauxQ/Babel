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
    User(Database& connection, const int id)
        : name_()
        , password_()
        , id_(id)
    {
        callbackData data{
            .name = name_,
            .password = password_,
            .used = false,
        };

        connection.exec("SELECT * FROM `users` WHERE id=" + std::to_string(id), takeUser, &data);
    }

    User(const User&) = delete;

    ~User() = default;

    [[nodiscard]] const std::string& getName() const
    {
        return name_;
    }

    [[nodiscard]] size_t getId() const
    {
        return id_;
    }

    [[nodiscard]] const std::string& getPassword() const
    {
        return password_;
    }

private:
    static int takeUser(void* toChange, int argc, char** argv, char** azColName)
    {
        auto data = static_cast<callbackData *>(toChange);

        data->used = true;
        data->password = std::string(argv[0]);
        data->name = std::string(argv[1]);

        return 0;
    }

    struct callbackData {
        std::string &name;
        std::string &password;
        bool used;
    };

    std::string name_;
    std::string password_;
    size_t id_;
};

#endif //BABEL_SERVER_USER_HPP
