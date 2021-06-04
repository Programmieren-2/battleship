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

#include <utility>
using std::pair;

#include "Coordinate.h"

namespace models {
    Coordinate::Coordinate(unsigned short x, unsigned short y)
        : std::pair<unsigned short, unsigned short>(x, y)
    {}

    Coordinate Coordinate::shift(unsigned short offset, Orientation orientation) const
    {
        switch (orientation) {
            case X:
                return Coordinate(first + offset, second);
            case Y:
                return Coordinate(first, second + offset);
        }
    }

    bool Coordinate::insideBounds(unsigned short x, unsigned short y) const
    {
        return first < x && second < y;
    }

    optional<Coordinate> Coordinate::fromString(const string &strX, const string &strY)
    {
        unsigned short x, y;

        try {
            x = static_cast<unsigned short>(stoul(strX));
        } catch (invalid_argument&) {
            return {};
        }

        try {
            y = static_cast<unsigned short>(stoul(strY));
        } catch (invalid_argument&) {
            return {};
        }

        return Coordinate(x, y);
    }
}