//
// Created by alex on 9/23/19.
//

#ifndef BABEL_SERVER_SERVERERROR_HPP
#define BABEL_SERVER_SERVERERROR_HPP

#include <exception>
#include <sqlite3.h>

class ServerError : public std::exception {
public:
    explicit ServerError(std::exception* error)
        : error_(error)
    {
    }

    [[nodiscard]] const char* what() const noexcept final
    {
        return error_->what();
    }

private:
    std::exception* error_;
};

#endif //BABEL_SERVER_SERVERERROR_HPP
