//
// Created by rne on 07.05.21.
//

#include "ShipTypes.h"

namespace models {
    SpeedBoat::SpeedBoat(const Coordinate &anchorPoint, Orientation orientation)
            : Ship("Speed boat", anchorPoint, 2, orientation)
    {}

    Destroyer::Destroyer(const Coordinate &anchorPoint, Orientation orientation)
            : Ship("Destroyer", anchorPoint, 3, orientation)
    {}

    Submarine::Submarine(const Coordinate &anchorPoint, Orientation orientation)
            : Ship("Submarine", anchorPoint, 3, orientation)
    {}

    Battleship::Battleship(const Coordinate &anchorPoint, Orientation orientation)
            : Ship("Battleship", anchorPoint, 4, orientation)
    {}

    AircraftCarrier::AircraftCarrier(const Coordinate &anchorPoint, Orientation orientation)
            : Ship("Aircraft carrier", anchorPoint, 5, orientation)
    {}
}