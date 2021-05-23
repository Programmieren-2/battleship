//
// Created by rne on 07.05.21.
//

#include <optional>
using std::optional;

#include <stdexcept>
using std::invalid_argument;

#include <string>
using std::stoul;
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

    bool Coordinate::operator==(Coordinate const &coordinate) const
    {
        return x == coordinate.x && y == coordinate.y;
    }

    optional<Coordinate> Coordinate::fromString(const string &strX, const string &strY)
    {
        optional<Coordinate> coordinate;
        unsigned short x, y;

        try {
            x = static_cast<unsigned short>(stoul(strX));
        } catch (invalid_argument&) {
            return coordinate;
        }

        try {
            y = static_cast<unsigned short>(stoul(strY));
        } catch (invalid_argument&) {
            return coordinate;
        }

        return coordinate = Coordinate(x, y);
    }
}