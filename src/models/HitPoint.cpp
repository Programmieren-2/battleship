//
// Created by rne on 07.05.21.
//

#include "Coordinate.h"

#include "HitPoint.h"

namespace models {
    HitPoint::HitPoint(unsigned short x, unsigned short y)
            : Coordinate(x, y), hit(false)
    {}

    HitResult HitPoint::doHit()
    {
        if (hit)
            return HitResult::ALREADY_HIT;

        hit = true;
        return HitResult::HIT;
    }

    bool HitPoint::isHit() const
    {
        return hit;
    }
}