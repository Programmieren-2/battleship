//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_BOOTSTRAP_H
#define BATTLESHIP_BOOTSTRAP_H

#include <string>

#include "Coordinate.h"
#include "PlayerBoard.h"

namespace bootstrap {
    models::Coordinate readCoordinate(std::string const &prompt);
    models::Coordinate readCoordinate();
    models::Orientation readOrientation(string const &prompt);
    models::Orientation readOrientation();
    models::PlayerBoard readPlayerBoard();
}


#endif //BATTLESHIP_BOOTSTRAP_H
