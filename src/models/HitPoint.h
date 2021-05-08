//
// Created by rne on 07.05.21.
//

#ifndef BATTLESHIP_HITPOINT_H
#define BATTLESHIP_HITPOINT_H

#include "Coordinate.h"

namespace models {
    enum HitResult { HIT, ALREADY_HIT, MISSED };

    class HitPoint : public Coordinate {
    private:
        bool hit;
    public:
        HitPoint(unsigned short x, unsigned short y);

        HitResult doHit();
        bool isHit() const;
    };
}

#endif //BATTLESHIP_HITPOINT_H
