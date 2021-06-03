//
// Created by rne on 07.05.21.
//

#pragma once

#include <map>
#include <string>
#include <vector>

namespace models {
    enum PlacementResult : unsigned char {
        SUCCESS,
        NOT_ON_BOARD,
        COLLISION,
        ALREADY_PLACED,
        INVALID_SHIP_TYPE
    };
    enum HitResult : unsigned char {
        HIT,
        ALREADY_HIT,
        MISSED
    };

    using ShipTypes = std::map<std::string, unsigned short>;

    namespace Defaults {
        static const unsigned short WIDTH = 10;
        static const unsigned short HEIGHT = 10;
        static const ShipTypes SHIP_TYPES = {
                {"Speed boat", static_cast<unsigned short>(2)},
                {"Destroyer", static_cast<unsigned short>(3)},
                {"Submarine", static_cast<unsigned short>(3)},
                {"Battleship", static_cast<unsigned short>(4)},
                {"Aircraft Carrier", static_cast<unsigned short>(5)},
        };
    }
}