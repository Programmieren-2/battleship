//
// Created by rne on 15.05.21.
//

#pragma once

#include <string>

#include "Coordinate.h"

namespace models {
    class BasicShip {
    private:
        std::string type;
        Coordinate anchorPoint;
        Orientation orientation;
    public:
        BasicShip(std::string type, Coordinate anchorPoint, Orientation const &orientation);

        [[nodiscard]] std::string getType() const;
        [[nodiscard]] Coordinate getAnchorPoint() const;
        [[nodiscard]] Orientation getOrientation() const;

        bool operator==(BasicShip const &other) const;
        bool operator!=(BasicShip const &other) const;
    };
}