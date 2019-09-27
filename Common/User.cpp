//
// Created by alex on 9/23/19.
//

#include "User.hpp"

std::ostream& operator<<(std::ostream& os, const User& user)
{
    os << "User {" << std::endl;
    os << "  username: " << user.name_ << std::endl;
    os << "  (hashed) password: " << user.password_ << std::endl;
    os << "  id: " << user.id_ << std::endl;
    os << "}" << std::endl;
    return os;
}

int User::takeUser(void* toChange, int argc, char** argv, char** azColName)
{
    auto data = static_cast<callbackData*>(toChange);

    data->used = true;
    data->id = std::stoll(argv[0]);
    data->name = std::string(argv[1]);
    data->password = std::string(argv[2]);
    return 0;
}

User::User(Database& connection, int id)
    : name_()
    , password_()
    , id_(id)
{
    callbackData data {
        .name = name_,
        .password = password_,
        .id = id_,
        .used = false,
    };

    connection.exec("SELECT * FROM `users` WHERE id=" + std::to_string(id), takeUser, &data);

    if (!data.used)
        throw DatabaseError(ROW_NOT_FOUND);
}

User::User(Database& connection, const std::string& username)
    : id_(0)
    , name_()
    , password_()
{
    callbackData data {
        .name = name_,
        .password = password_,
        .id = id_,
        .used = false,
    };

    connection.exec("SELECT * FROM `users` WHERE username='" + username + "'", takeUser, &data);

    if (!data.used) {
        throw DatabaseError(ROW_NOT_FOUND);
    }
}
