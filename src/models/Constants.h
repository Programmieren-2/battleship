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
                {"Speed boat", (unsigned short) 2},
                {"Destroyer", (unsigned short) 3},
                {"Submarine", (unsigned short) 3},
                {"Battleship", (unsigned short) 4},
                {"Aircraft Carrier", (unsigned short) 5},
        };
    }
}
#endif //BATTLESHIP_CONSTANTS_H
