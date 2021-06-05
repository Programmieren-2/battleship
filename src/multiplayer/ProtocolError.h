//
// Created by rne on 15.05.21.
//

#pragma once

#include <exception>
#include <string>

#include "Messages.h"

namespace multiplayer {
    class ProtocolError : public std::exception {
    private:
        ErrorType type;
        std::string message;
    public:
        explicit ProtocolError(ErrorType type);

        [[nodiscard]] ErrorType getType() const noexcept;
        [[nodiscard]] const char *what() const noexcept;
    };
}
