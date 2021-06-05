//
// Created by rne on 15.05.21.
//

#include <exception>
using std::exception;

#include <string>
using std::string;
using std::to_string;

#include "Messages.h"
#include "ProtocolError.h"

namespace multiplayer {
    ProtocolError::ProtocolError(ErrorType type)
        : exception(), type(type)
    {
        message = "Unexpected response type: " + to_string(type);
    }

    ErrorType ProtocolError::getType() const noexcept
    {
        return type;
    }

    const char *ProtocolError::what() const noexcept
    {
        return message.c_str();
    }
}
