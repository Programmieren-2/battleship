//
// Created by rne on 07.05.21.
//

#include "Coordinate.h"

#include "HitPoint.h"

namespace models {
    HitPoint::HitPoint(unsigned short x, unsigned short y)
        : Coordinate(x, y), hit(false)
    {}

    HitPoint::HitPoint(Coordinate const &coordinate)
        : HitPoint(coordinate.first, coordinate.second)
    {}

    bool HitPoint::isHit() const
    {
        return hit;
    }

    HitResult HitPoint::doHit()
    {
        if (hit)
            return ALREADY_HIT;

        hit = true;
        return HIT;
    }
}