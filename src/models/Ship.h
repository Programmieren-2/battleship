//
// Created by rne on 07.05.21.
//

#pragma once

#include <string>
#include <vector>

#include "BasicShip.h"
#include "Coordinate.h"
#include "HitPoint.h"

namespace models {
    class Ship : public BasicShip {
    private:
        unsigned short length;
        std::vector<HitPoint> hitPoints;

        void initializeHitPoints();
    public:
        Ship(std::string const &type, Coordinate const &anchorPoint, unsigned short length, Orientation orientation);

        [[nodiscard]] unsigned short getLength() const;
        [[nodiscard]] Coordinate getEndPoint() const;

        [[nodiscard]] bool occupies(Coordinate const &coordinate) const;
        [[nodiscard]] bool collidesWith(Ship const &ship) const;
        [[nodiscard]] bool isHitAt(Coordinate const &coordinate) const;
        [[nodiscard]] bool isDestroyed() const;

        HitResult fireAt(Coordinate const &coordinate);
    };
}