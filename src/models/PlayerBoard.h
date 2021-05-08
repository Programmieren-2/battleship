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

    class PlayerBoard : public Board {
    private:
        std::string name;
        Ships ships;
        Coordinates misses;

        bool coordinateOnBoard(Coordinate const &coordinate) const;
        bool shipOnBoard(Ship const &ship) const;
        bool shipCollides(Ship const &ship) const;
        std::string getCharAt(Coordinate const &coordinate, bool showShips) const;
    public:
        PlayerBoard(std::string name, unsigned short width, unsigned short height);
        PlayerBoard(std::string name);

        std::string getName() const;
        bool allShipsDestroyed() const;
        std::string toString(bool showShips) const;
        std::string toString() const;

        PlacementResult placeShip(Ship const &ship);
        HitResult fireAt(Coordinate const &coordinate);
    };

    typedef std::vector<PlayerBoard> PlayerBoards;
}

#endif //BATTLESHIP_PLAYERBOARD_H
