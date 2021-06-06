//
// Created by rne on 07.05.21.
//

#include <algorithm>
using std::ranges::all_of;
using std::ranges::any_of;

#include <optional>
using std::optional;

#include <stdexcept>
using std::invalid_argument;
using std::out_of_range;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <utility>
using std::move;

#include "BasicShip.h"
#include "Coordinate.h"
#include "HitPoint.h"
#include "Ship.h"

namespace models {
    Ship::Ship(string const &type, Coordinate const &anchorPoint, unsigned short length, Orientation const &orientation)
        : BasicShip(type, anchorPoint, orientation), length(length), hitPoints(vector<HitPoint>(length, HitPoint()))
    {}

    unsigned short Ship::getLength() const
    {
        return length;
    }

    vector<Coordinate> Ship::getCoordinates() const
    {
        vector<Coordinate> coordinates;
        Coordinate const &anchorPoint = getAnchorPoint();

        for (unsigned int offset = 0; offset < length; ++offset)
            coordinates.push_back(anchorPoint.shift(offset, getOrientation()));

        return coordinates;
    }

    unsigned short Ship::getHitPointIndex(Coordinate const &coordinate) const
    {
        Coordinate const &anchorPoint = getAnchorPoint();
        int offset;

        switch (getOrientation()) {
            case X:
                if (coordinate.first != anchorPoint.first)
                    throw out_of_range("X-coordinates do not match.");

                offset = coordinate.second - anchorPoint.second;
                if (offset < 0 || offset >= length)
                    throw out_of_range("Y-coordinate does not match.");

                return offset;
            case Y:
                if (coordinate.second != anchorPoint.second)
                    throw out_of_range("Y-coordinates do not match.");

                offset = coordinate.first - anchorPoint.first;
                if (offset < 0 || offset >= length)
                    throw out_of_range("X-coordinate does not match.");

                return offset;
            default:
                throw invalid_argument("Invalid orientation.");
        }
    }

    HitPoint const & Ship::getHitPointAt(Coordinate const &coordinate) const
    {
        return hitPoints.at(getHitPointIndex(coordinate));
    }

    HitPoint& Ship::getHitPointAt(Coordinate const &coordinate)
    {
        return hitPoints.at(getHitPointIndex(coordinate));
    }

    Coordinate Ship::getEndPoint() const
    {
        return getAnchorPoint().shift(length - 1, getOrientation());
    }

    bool Ship::occupies(const Coordinate &coordinate) const
    {
        return any_of(getCoordinates(), [coordinate] (auto const &position) { return position == coordinate; });
    }

    bool Ship::collidesWith(Ship const &other) const
    {
        return any_of(getCoordinates(), [other] (auto const &coordinate) { return other.occupies(coordinate); });
    }

    bool Ship::isHitAt(Coordinate const &coordinate) const
    {
        try {
            return getHitPointAt(coordinate).isHit();
        } catch (out_of_range const &) {
            return false;
        }
    }

    bool Ship::isDestroyed() const
    {
        return all_of(hitPoints, [] (HitPoint const &hitPoint) {
            return hitPoint.isHit();
        });
    }

    HitResult Ship::fireAt(const Coordinate &coordinate)
    {
        try {
            return getHitPointAt(coordinate).doHit();
        } catch (out_of_range const &) {
            return MISSED;
        }
    }
}