//
// Created by rne on 07.05.21.
//

#ifndef BATTLESHIP_BOARD_H
#define BATTLESHIP_BOARD_H

#include "Coordinate.h"

namespace models {
    class Board {
    private:
        unsigned int width;
        unsigned int height;
    public:
        Board(unsigned int width, unsigned int height);

        unsigned int getWidth() const;
        unsigned int getHeight() const;
        models::Coordinates getCoordinates() const;
    };
}


#endif //BATTLESHIP_BOARD_H
