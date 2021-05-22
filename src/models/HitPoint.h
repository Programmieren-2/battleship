//
// Created by rne on 07.05.21.
//

#pragma once

#include "Models.h"
#include "Coordinate.h"

namespace models {
    class HitPoint : public Coordinate {
    private:
        bool hit;
    public:
        HitPoint(unsigned short x, unsigned short y);

        [[nodiscard]] bool isHit() const;

        [[nodiscard]] HitResult doHit();
    };
}