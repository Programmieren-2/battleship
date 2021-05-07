//
// Created by rne on 07.05.21.
//

#ifndef BATTLESHIP_PLAYERBOARD_H
#define BATTLESHIP_PLAYERBOARD_H

#include <vector>

#include "Board.h"
#include "Ship.h"

namespace models {
    enum PlacementResult { SUCCESS, NOT_ON_BOARD, COLLISION };

    class PlayerBoard : Board {
    private:
        unsigned short width;
        unsigned short height;
        Ships ships;

        bool coordinateOnBoard(Coordinate const &coordinate) const;
        bool shipOnBoard(Ship const &ship) const;
        bool shipCollides(Ship const &ship) const;
        PlacementResult checkPlacement(Ship const &ship) const;
    public:
        PlayerBoard(unsigned short width, unsigned short height, Ships ships);

        bool placeShip(Ship const &ship);
        bool fireAt(Coordinate const &coordinate);
    };

    typedef std::vector<PlayerBoard> PlayerBoards;
}

#endif //BATTLESHIP_PLAYERBOARD_H
