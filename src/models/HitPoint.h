//
// Created by rne on 07.05.21.
//

#ifndef BATTLESHIP_HITPOINT_H
#define BATTLESHIP_HITPOINT_H

#include <vector>

#include "Coordinate.h"

namespace models {
    enum HitResult { HIT, ALREADY_HIT, MISSED };

    class HitPoint : public Coordinate {
    private:
        bool hit;
    public:
        HitPoint(unsigned short x, unsigned short y);

        [[nodiscard]] HitResult doHit();
        [[nodiscard]] bool isHit() const;
    };

    typedef std::vector<HitPoint> HitPoints;
    typedef std::vector<HitPoints> Grid;
}

#endif //BATTLESHIP_HITPOINT_H
