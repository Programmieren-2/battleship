//
// Created by rne on 07.05.21.
//

#pragma once

#include <map>
#include <string>
#include <vector>

#include "ShipTypes.h"

namespace models::Defaults {
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