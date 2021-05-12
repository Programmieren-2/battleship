//
// Created by rne on 12.05.21.
//

#ifndef BATTLESHIP_EXCEPTIONS_H
#define BATTLESHIP_EXCEPTIONS_H

#include <exception>
#include <string>

namespace proto {
    class BufferSizeMismatch : public std::exception {
    private:
        const char *reason;
    public:
        explicit BufferSizeMismatch(const char *reason);
        [[nodiscard]] const char *what () const noexcept override;
    };
}

#endif //BATTLESHIP_EXCEPTIONS_H
