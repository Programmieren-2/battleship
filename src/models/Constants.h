//
// Created by rne on 07.05.21.
//

#ifndef BATTLESHIP_CONSTANTS_H
#define BATTLESHIP_CONSTANTS_H

#include <map>
#include <string>
#include <vector>

#include "Ship.h"

namespace models {
    typedef std::map<std::string, unsigned short> ShipTypes;

    namespace Constants {
        static const unsigned short width = 10;
        static const unsigned short height = 10;
        static const ShipTypes shipTypes = {
                {"Speed boat", static_cast<unsigned short>(2)},
                {"Destroyer", static_cast<unsigned short>(3)},
                {"Submarine", static_cast<unsigned short>(3)},
                {"Battleship", static_cast<unsigned short>(4)},
                {"Aircraft Carrier", static_cast<unsigned short>(5)},
        };
        static const std::vector<std::string> VALID_PLAYER_NAMES = {"Richard", "Steve"};
    }
}
#endif //BATTLESHIP_CONSTANTS_H
