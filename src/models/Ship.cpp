//
// Created by rne on 07.05.21.
//

#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

#include "Coordinate.h"
#include "HitPoint.h"
#include "Ship.h"

namespace models {
    Ship::Ship(string type, Coordinate const &anchorPoint, unsigned short length, Orientation orientation)
            : type(type), anchorPoint(anchorPoint), length(length), orientation(orientation), hitPoints(HitPoints())
    {
        for (unsigned int offset = 0; offset < length; offset++) {
            switch (orientation) {
                case Orientation::X:
                    hitPoints.push_back(HitPoint(anchorPoint.getX() + offset, anchorPoint.getY()));
                    break;
                case Orientation::Y:
                    hitPoints.push_back(HitPoint(anchorPoint.getX(), anchorPoint.getY() + offset));
                    break;
            }
        }
    }

    Coordinate Ship::getAnchorPoint() const
    {
        return anchorPoint;
    }

    Coordinate Ship::getEndPoint() const
    {
        if (orientation == Orientation::X)
            return Coordinate(anchorPoint.getX() + length - 1, anchorPoint.getY());

        return Coordinate(anchorPoint.getX(), anchorPoint.getY() + length - 1);
    }

    HitPoints Ship::getHitPoints() const
    {
        return hitPoints;
    }

    bool Ship::collidesWith(const Ship &ship) const
    {
        for (HitPoint hitPoint : hitPoints) {
            for (HitPoint otherHitPoint : ship.getHitPoints()) {
                if (hitPoint == otherHitPoint)
                    return true;
            }
        }

        return false;
    }

    bool Ship::occupiesPosition(const Coordinate &coordinate) const
    {
        for (HitPoint hitPoint : hitPoints) {
            if (hitPoint == coordinate)
                return true;
        }

        return false;
    }

    bool Ship::isHit() const
    {
        for (HitPoint hitPoint : hitPoints) {
            if (hitPoint.isHit())
                return true;
        }

        return false;
    }

    bool Ship::isDestroyed() const
    {
        for (HitPoint hitPoint : hitPoints) {
            if (!hitPoint.isHit())
                return false;
        }

        return true;
    }

    HitResult Ship::hitAt(const Coordinate &coordinate)
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

    string Ship::toString() const
    {
        return type + " (" + anchorPoint.toString() + ", " + getEndPoint().toString() + ")";
    }

    bool Ship::operator==(const Ship &other) const
    {
        return type == other.getType();
    }

    bool Ship::operator!=(const Ship &other) const
    {
        return !(*this == other);
    }
}