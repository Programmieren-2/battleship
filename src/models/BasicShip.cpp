//
// Created by rne on 15.05.21.
//

#include <string>
using std::string;

#include <utility>
using std::move;

#include "Coordinate.h"
#include "BasicShip.h"

namespace models {
    BasicShip::BasicShip(string type, Coordinate anchorPoint, Orientation const &orientation)
        : type(move(type)), anchorPoint(move(anchorPoint)), orientation(orientation)
    {}

    string BasicShip::getType() const
    {
        return type;
    }

    Coordinate BasicShip::getAnchorPoint() const
    {
        return anchorPoint;
    }

    Orientation BasicShip::getOrientation() const
    {
        return orientation;
    }

    bool BasicShip::operator==(BasicShip const &other) const
    {
        return type == other.getType();
    }

    bool BasicShip::operator!=(BasicShip const &other) const
    {
        return !(*this == other);
    }
}