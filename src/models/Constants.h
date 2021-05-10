//
// Created by rne on 07.05.21.
//

#ifndef BATTLESHIP_CONSTANTS_H
#define BATTLESHIP_CONSTANTS_H

#include <map>
#include <string>

#include "Ship.h"

namespace models {
    typedef std::map<std::string, unsigned short> ShipTypes;

    namespace Constants {
        static const unsigned short width = 10;
        static const unsigned short height = 10;
        static const ShipTypes shipTypes = {
                {"Speed boat", 2},
                {"Destroyer", 3},
                {"Submarine", 3},
                {"Battleship", 4},
                {"Aircraft Carrier", 5},
        };
    }
}
#endif //BATTLESHIP_CONSTANTS_H
