//
// Created by rne on 09.06.21.
//

#include <iostream>
using std::ostream;

#include <string>
using std::string;

#include <utility>
using std::move;

#include "CLIError.h"

namespace multiplayer{
    CLIError::CLIError(string message)
        : message(move(message))
    {}

    char const *CLIError::what() const noexcept
    {
        return message.c_str();
    }

    ostream& operator<<(ostream &out, CLIError const &error)
    {
        out << error.message;
        return out;
    }
}