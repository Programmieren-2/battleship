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
        size_t actualSize;
        size_t expectedSize;
    public:
        BufferSizeMismatch(size_t actualSize, size_t expectedSize);
        [[nodiscard]] size_t getActualSize() const;
        [[nodiscard]] size_t getExpectedSize() const;
        [[nodiscard]] string getMessage() const;
        [[nodiscard]] const char *what () const noexcept override;
    };
}

#endif //BATTLESHIP_EXCEPTIONS_H
