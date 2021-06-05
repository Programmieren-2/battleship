//
// Created by rne on 05.06.21.
//

#include <string>
using std::to_string;

#include "exceptions.h"

namespace models {
    OutsideSeaBounds::OutsideSeaBounds(unsigned short width, unsigned short height)
        : message("Ship is outside the sea bounds (" + to_string(width) + "x" + to_string(height) + ").")
    {}

    char const *OutsideSeaBounds::what() const noexcept
    {
        return message.c_str();
    }

    char const *Collision::what() const noexcept
    {
        return "Ship collides with another ship.";
    }
}