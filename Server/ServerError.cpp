//
// Created by alex on 9/23/19.
//

#include "ServerError.hpp"

ServerError::ServerError(std::exception* error)
    : error_(error) {};

const char* ServerError::what() const noexcept
{
    return error_->what();
}
