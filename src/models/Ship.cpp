//
// Created by rne on 07.05.21.
//

#include <algorithm>
using std::all_of;

#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

#include "Coordinate.h"
#include "HitPoint.h"
#include "Ship.h"

namespace models {
    Ship::Ship(string const &type, Coordinate const &anchorPoint, unsigned short length, Orientation const orientation)
            : type(type), anchorPoint(anchorPoint), length(length), orientation(orientation), hitPoints(HitPoints())
    {
        for (unsigned short offset = 0; offset < length; offset++) {
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

    bool Ship::collidesWith(Ship const &ship) const
    {
        for (HitPoint const &hitPoint : hitPoints) {
            for (HitPoint const &otherHitPoint : ship.getHitPoints()) {
                if (hitPoint == otherHitPoint)
                    return true;
            }
        }

        return false;
    }

    bool Ship::isDestroyed() const
    {
        return all_of(hitPoints.begin(), hitPoints.end(), [](HitPoint const &hitPoint) {
            return hitPoint.isHit();
        });
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

    bool Ship::operator==(Ship const &other) const
    {
        return type == other.getType();
    }

    bool Ship::operator!=(Ship const &other) const
    {
        return !(*this == other);
    }
}