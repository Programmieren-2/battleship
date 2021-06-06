//
// Created by rne on 07.05.21.
//

#include "HitPoint.h"
#include "HitResult.h"

namespace models {
    HitPoint::HitPoint()
        : hit(false)
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