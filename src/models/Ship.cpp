//
// Created by rne on 07.05.21.
//

#include <algorithm>
using std::all_of;
using std::any_of;

#include <optional>
using std::optional;

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
        optional<HitPoint> hitPoint;

        for (unsigned short offset = 0; offset < length; offset++) {
            Coordinate coordinate = getAnchorPoint();

            switch (getOrientation()) {
                case Orientation::X:
                    hitPoint = HitPoint(coordinate.getX() + offset, coordinate.getY());
                    break;
                case Orientation::Y:
                    hitPoint = HitPoint(coordinate.getX(), coordinate.getY() + offset);
                    break;
                }

            if (hitPoint.has_value())
                hitPoints.push_back(hitPoint.value());
        }
    }

    unsigned short Ship::getLength() const
    {
        return length;
    }

    Coordinate Ship::getEndPoint() const
    {
        Coordinate coordinate = getAnchorPoint();

        switch (getOrientation()) {
            case Orientation::X:
                return Coordinate(coordinate.getX() + length - 1, coordinate.getY());
            case Orientation::Y:
                return Coordinate(coordinate.getX(), coordinate.getY() + length - 1);
        }

        throw "Invalid orientation.";
    }

    bool Ship::occupies(const Coordinate &coordinate) const
    {
        return any_of(hitPoints.begin(), hitPoints.end(), [coordinate](auto const &hitPoint) {
            return hitPoint == coordinate;
        });
    }

    bool Ship::collidesWith(Ship const &other) const
    {
        return any_of(hitPoints.begin(), hitPoints.end(), [other](auto const &coordinate) {
            return other.occupies(coordinate);
        });
    }

    bool Ship::isHitAt(Coordinate const &coordinate) const
    {
        return any_of(hitPoints.begin(), hitPoints.end(), [coordinate](auto const &hitPoint) {
            return hitPoint == coordinate && hitPoint.isHit();
        });
    }

    bool Ship::isDestroyed() const
    {
        return all_of(hitPoints.begin(), hitPoints.end(), [](HitPoint const &hitPoint) {
            return hitPoint.isHit();
        });
    }

    HitResult Ship::fireAt(const Coordinate &coordinate)
    {
        for (HitPoint &hitPoint : hitPoints) {
            if (hitPoint == coordinate)
                return hitPoint.doHit();
        }

        return HitResult::MISSED;
    }
}