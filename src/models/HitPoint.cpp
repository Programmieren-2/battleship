//
// Created by rne on 07.05.21.
//

#include "Coordinate.h"
#include "HitPoint.h"

namespace models {
    HitPoint::HitPoint(unsigned short x, unsigned short y)
            : Coordinate(x, y), hit(false)
    {}

    bool HitPoint::doHit()
    {
        if (hit)
            return false;

        hit = true;
        return true;
    }

    bool HitPoint::isHit() const
    {
        return hit;
    }
}