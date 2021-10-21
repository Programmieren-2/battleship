//
// Created by rne on 07.05.21.
//

#include <algorithm>
using std::ranges::all_of;
using std::ranges::any_of;

#include <iostream>
using std::cout;

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

    unsigned short Ship::getHitPointIndexX(Coordinate const &coordinate) const
    {
        Coordinate const &startingPoint = getAnchorPoint();

        if (coordinate.second != startingPoint.second)
            throw out_of_range("Y-coordinates do not match.");

        int offset = coordinate.first - startingPoint.first;
        if (offset < 0 || offset >= length)
            throw out_of_range("X-coordinate does not match.");

        return static_cast<unsigned short>(offset);
    }

    unsigned short Ship::getHitPointIndexY(Coordinate const &coordinate) const
    {
        Coordinate const &startingPoint = getAnchorPoint();

        if (coordinate.first != startingPoint.first)
            throw out_of_range("X-coordinates do not match.");

        int offset = coordinate.second - startingPoint.second;
        if (offset < 0 || offset >= length)
            throw out_of_range("Y-coordinate does not match.");

        return static_cast<unsigned short>(offset);
    }

    unsigned short Ship::getHitPointIndex(Coordinate const &coordinate) const
    {
        switch (getOrientation()) {
            case X:
                return getHitPointIndexX(coordinate);
            case Y:
                return getHitPointIndexY(coordinate);
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

    vector<Coordinate> Ship::getCoordinates() const
    {
        vector<Coordinate> coordinates;
        Coordinate const &startingPoint = getAnchorPoint();

        for (unsigned short offset = 0; offset < length; ++offset)
            coordinates.push_back(startingPoint.shift(offset, getOrientation()));

        return coordinates;
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
        return all_of(hitPoints, [] (HitPoint const &hitPoint) { return hitPoint.isHit(); });
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