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

#include <string>
using std::string;

#include <utility>
using std::move;

#include "BasicShip.h"
#include "Coordinate.h"
#include "HitPoint.h"
#include "Ship.h"

namespace models {
    Ship::Ship(string const &type, Coordinate const &anchorPoint, unsigned short length, Orientation orientation)
        : BasicShip(type, anchorPoint, orientation), length(length)
    {
        initializeHitPoints();
    }

    void Ship::initializeHitPoints()
    {
        for (unsigned short offset = 0; offset < length; ++offset) {
            hitPoints.emplace_back(getAnchorPoint().shift(offset, getOrientation()));
        }
    }

    unsigned short Ship::getLength() const
    {
        return length;
    }

    Coordinate Ship::getEndPoint() const
    {
        return getAnchorPoint().shift(length - 1, getOrientation());
    }

    bool Ship::occupies(const Coordinate &coordinate) const
    {
        return any_of(hitPoints, [coordinate] (auto const &hitPoint) { return hitPoint == coordinate; });
    }

    bool Ship::collidesWith(Ship const &other) const
    {
        return any_of(hitPoints, [other] (auto const &coordinate) { return other.occupies(coordinate); });
    }

    bool Ship::isHitAt(Coordinate const &coordinate) const
    {
        return any_of(hitPoints, [coordinate] (auto const &hitPoint) {
            return hitPoint == coordinate && hitPoint.isHit();
        });
    }

    bool Ship::isDestroyed() const
    {
        return all_of(hitPoints, [] (HitPoint const &hitPoint) {
            return hitPoint.isHit();
        });
    }

    HitResult Ship::fireAt(const Coordinate &coordinate)
    {
        for (auto &hitPoint : hitPoints) {
            if (hitPoint == coordinate)
                return hitPoint.doHit();
        }

        return MISSED;
    }
}