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

#include "Coordinate.h"
#include "HitPoint.h"
#include "Ship.h"

namespace models {
    Ship::Ship(string type, Coordinate const &anchorPoint, unsigned short length, Orientation orientation)
            : type(move(type)), anchorPoint(anchorPoint), length(length), orientation(orientation), hitPoints(HitPoints())
    {
        initializeGrid();
    }

    void Ship::initializeGrid()
    {
        optional<HitPoint> hitPoint;

        for (unsigned short offset = 0; offset < length; offset++) {
            switch (orientation) {
            case Orientation::X:
                hitPoint = HitPoint(anchorPoint.getX() + offset, anchorPoint.getY());
                break;
            case Orientation::Y:
                hitPoint = HitPoint(anchorPoint.getX(), anchorPoint.getY() + offset);
                break;
            }

            hitPoints.push_back(hitPoint.value());
        }
    }

    Coordinate Ship::getAnchorPoint() const
    {
        return anchorPoint;
    }

    Coordinate Ship::getEndPoint() const
    {
        switch (orientation) {
        case Orientation::X:
            return Coordinate(anchorPoint.getX() + length - 1, anchorPoint.getY());
        case Orientation::Y:
            return Coordinate(anchorPoint.getX(), anchorPoint.getY() + length - 1);
        }
    }

    HitPoints Ship::getHitPoints() const
    {
        return hitPoints;
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

    string Ship::getType() const
    {
        return type;
    }

    bool Ship::operator==(Ship const &other) const
    {
        return type == other.getType();
    }

    bool Ship::operator!=(Ship const &other) const
    {
        return !(*this == other);
    }
}