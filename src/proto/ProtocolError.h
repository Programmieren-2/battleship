//
// Created by rne on 15.05.21.
//

#ifndef BATTLESHIP_PROTOCOLERROR_H
#define BATTLESHIP_PROTOCOLERROR_H

#include <exception>
#include <string>

#include "Messages.h"

namespace proto {
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

#endif //BATTLESHIP_PROTOCOLERROR_H
