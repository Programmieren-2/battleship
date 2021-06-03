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
    Coordinate::Coordinate(uint8_t x, uint8_t y)
        : std::pair<uint8_t, uint8_t>(x, y)
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
        uint8_t x, y;

        try {
            x = static_cast<uint8_t>(stoul(strX));
        } catch (invalid_argument&) {
            return {};
        }

        try {
            y = static_cast<uint8_t>(stoul(strY));
        } catch (invalid_argument&) {
            return {};
        }

        return Coordinate(x, y);
    }
}