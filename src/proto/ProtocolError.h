//
// Created by rne on 15.05.21.
//

#ifndef BATTLESHIP_PROTOCOLERROR_H
#define BATTLESHIP_PROTOCOLERROR_H

#include <exception>
#include <string>

#include "Messages.h"

namespace proto {
    class ProtocolError : std::exception {
    private:
        ResponseType type;
        std::string message;
    public:
        explicit ProtocolError(ResponseType type);

        const char *what() const noexcept;
    };
}

#endif //BATTLESHIP_PROTOCOLERROR_H
