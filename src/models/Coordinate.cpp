//
// Created by rne on 07.05.21.
//

#include <string>
using std::string;
using std::to_string;

#include "Coordinate.h"

namespace models {
    Coordinate::Coordinate(unsigned short x, unsigned short y)
            : x(x), y(y)
    {}

    unsigned short Coordinate::getX() const
    {
        return x;
    }

    unsigned short Coordinate::getY() const
    {
        return y;
    }

    bool Coordinate::operator==(const Coordinate &coordinate) const
    {
        return x == coordinate.x && y == coordinate.y;
    }
}