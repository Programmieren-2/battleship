//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_BOOTSTRAP_H
#define BATTLESHIP_BOOTSTRAP_H

#include <string>

namespace bootstrap {
    models::Coordinate readCoordinate(std::string const &prompt);
    models::Orientation readOrientation(std::string const &prompt);
    void readShip(models::PlayerBoard &playerBoard, std::string type, unsigned short length);
    void readShips(models::PlayerBoard &playerBoard);
    models::PlayerBoard readPlayerBoard();
}


#endif //BATTLESHIP_BOOTSTRAP_H
