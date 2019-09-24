//
// Created by alex on 9/24/19.
//

#ifndef BABEL_SERVER_DATABASEERROR_HPP
#define BABEL_SERVER_DATABASEERROR_HPP

#include <exception>
#include <sqlite3.h>

class DatabaseError : public std::exception {
public:
    explicit DatabaseError(int error)
        : error_(error)
    {
    }

    [[nodiscard]] const char* what() const noexcept final
    {
        return sqlite3_errstr(error_);
    }

public:
    int error_;
};

#endif //BABEL_SERVER_DATABASEERROR_HPP
