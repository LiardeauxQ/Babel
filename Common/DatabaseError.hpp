//
// Created by alex on 9/24/19.
//

#ifndef BABEL_SERVER_DATABASEERROR_HPP
#define BABEL_SERVER_DATABASEERROR_HPP

#include <exception>
#include <sqlite3.h>

static const int ROW_NOT_FOUND = -1;

class DatabaseError : public std::exception {
public:
    explicit DatabaseError(int error)
        : error_(error)
    {
    }

    [[nodiscard]] const char* what() const noexcept final
    {
        if (error_ > 0) {
            return sqlite3_errstr(error_);
        } else {
            switch (error_) {
            case ROW_NOT_FOUND:
                return "Row not found.";
            default:
                return "Unknown error.";
            }
        }
    }

public:
    int error_;
};

#endif //BABEL_SERVER_DATABASEERROR_HPP
