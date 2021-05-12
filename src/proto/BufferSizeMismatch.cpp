//
// Created by rne on 12.05.21.
//

#include <string>
using std::string;
using std::to_string;

#include "BufferSizeMismatch.h"

namespace proto {
    BufferSizeMismatch::BufferSizeMismatch(const char *reason)
            : reason(reason)
    {}

    const char *BufferSizeMismatch::what () const noexcept
    {
        return reason;
    }
}