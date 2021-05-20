//
// Created by rne on 12.05.21.
//

#ifndef BATTLESHIP_SOCKETERROR_H
#define BATTLESHIP_SOCKETERROR_H

#include <exception>

#include "boostwrap.h"

namespace net {
    class SocketError : public std::exception {
    private:
        const boost::system::error_code error;
    public:
        explicit SocketError(boost::system::error_code error);
        [[nodiscard]] boost::system::error_code getError() const noexcept;
        [[nodiscard]] const char *what() const noexcept override;
    };
}

#endif //BATTLESHIP_SOCKETERROR_H
