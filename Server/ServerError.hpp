//
// Created by alex on 9/23/19.
//

#ifndef BABEL_SERVER_SERVERERROR_HPP
#define BABEL_SERVER_SERVERERROR_HPP

#include <exception>
#include <sqlite3.h>

class ServerError : public std::exception {
public:
    enum ErrorType {
        Sqlite
    };

    explicit ServerError(int error, ErrorType type)
        : error_(error)
        , type_(type)
    {
    }

    [[nodiscard]] const char* what() const noexcept final {
        switch (type_) {
        case Sqlite:
            return sqlite3_errstr(error_);
        default:
            return "Undefined error.";
        }
    }

private:
    int error_;
    ErrorType type_;
};

#endif //BABEL_SERVER_SERVERERROR_HPP
