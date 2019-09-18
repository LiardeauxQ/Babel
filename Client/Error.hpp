//
// Created by Quentin Liardeaux on 9/18/19.
//

#ifndef BABEL_SERVER_ERROR_HPP
#define BABEL_SERVER_ERROR_HPP

#include <exception>
#include <string>

class Error: public std::exception {
public:
    Error(const std::string &message);

    ~Error() = default;

    const char* what() const noexcept override { return message.data(); };

private:
    std::string message;
};

#endif //BABEL_SERVER_ERROR_HPP
