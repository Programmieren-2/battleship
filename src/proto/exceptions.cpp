//
// Created by rne on 12.05.21.
//

#include <string>
using std::string;
using std::to_string;

#include "exceptions.h"

namespace proto {
    BufferSizeMismatch::BufferSizeMismatch(size_t expectedSize, size_t actualSize)
            : expectedSize(expectedSize), actualSize(actualSize)
    {}

    size_t BufferSizeMismatch::getActualSize() const
    {
        return actualSize;
    }

    size_t BufferSizeMismatch::getExpectedSize() const
    {
        return expectedSize;
    }

    string BufferSizeMismatch::getMessage() const
    {
        if (expectedSize < actualSize)
            return "Packet larger than buffer size. Partial read: " + to_string(actualSize - expectedSize);

        if (actualSize < expectedSize)
            return "Packet smaller than buffer size. Bytes unprocessed: " + to_string(expectedSize - actualSize);

        return "Packet sizes match. Possibly invalid use of exception or a race condition.";
    }

    const char *BufferSizeMismatch::what () const throw()
    {
        return getMessage().c_str();
    }
}