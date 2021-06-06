//
// Created by rne on 07.05.21.
//

#pragma once

#include "Models.h"

namespace models {
    class HitPoint {
    private:
        bool hit;
    public:
        HitPoint();

        [[nodiscard]] bool isHit() const;

        [[nodiscard]] HitResult doHit();
    };
}