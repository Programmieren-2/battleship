//
// Created by rne on 07.05.21.
//

#ifndef BATTLESHIP_BOARD_H
#define BATTLESHIP_BOARD_H

#include "Coordinate.h"

namespace models {
    class Board {
    private:
        unsigned short width;
        unsigned short height;
    public:
        Board(unsigned short width, unsigned short height);

        unsigned short getWidth() const;
        unsigned short getHeight() const;
        models::Coordinates getCoordinates() const;
    };
}


#endif //BATTLESHIP_BOARD_H
