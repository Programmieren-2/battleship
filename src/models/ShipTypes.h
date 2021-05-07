//
// Created by rne on 07.05.21.
//

#ifndef BATTLESHIP_SHIPTYPES_H
#define BATTLESHIP_SHIPTYPES_H

#include "Coordinate.h"
#include "Ship.h"

namespace models {
    class SpeedBoat : public Ship {
    public:
        SpeedBoat(Coordinate const &anchorPoint, Orientation orientation);
    };

    class Destroyer : public Ship {
    public:
        Destroyer(Coordinate const &anchorPoint, Orientation orientation);
    };

    class Submarine : public Ship {
    public:
        Submarine(Coordinate const &anchorPoint, Orientation orientation);
    };

    class Battleship : public Ship {
    public:
        Battleship(Coordinate const &anchorPoint, Orientation orientation);
    };

    class AircraftCarrier : public Ship {
    public:
        AircraftCarrier(Coordinate const &anchorPoint, Orientation orientation);
    };
}

#endif //BATTLESHIP_SHIPTYPES_H
