//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_BOOTSTRAP_H
#define BATTLESHIP_BOOTSTRAP_H

#include <string>

namespace bootstrap {
    models::Coordinate readCoordinate(std::string const &prompt);
    models::PlayerBoard readPlayerBoard();
}


#endif //BATTLESHIP_BOOTSTRAP_H
