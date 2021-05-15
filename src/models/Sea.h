//
// Created by rne on 07.05.21.
//

#ifndef BATTLESHIP_SEA_H
#define BATTLESHIP_SEA_H

#include <string>
#include <vector>

#include "Coordinate.h"
#include "Ship.h"

namespace models {
    enum PlacementResult { SUCCESS, NOT_ON_BOARD, COLLISION, ALREADY_PLACED, INVALID_SHIP_TYPE };

    class Sea {
    private:
        unsigned short width;
        unsigned short height;
        Grid grid;
        Ships ships;

        void initializeGrid();
        [[nodiscard]] bool coordinateOnBoard(Coordinate const &coordinate) const;
        [[nodiscard]] bool shipOnBoard(Ship const &ship) const;
        [[nodiscard]] bool shipCollides(Ship const &ship) const;
        [[nodiscard]] std::string getSymbolAt(Coordinate const &coordinate, bool showShips) const;
    public:
        Sea(unsigned short width, unsigned short height);
        Sea();

        [[nodiscard]] unsigned short getWidth() const;
        [[nodiscard]] unsigned short getHeight() const;
        [[nodiscard]] std::string getName() const;
        [[nodiscard]] bool allShipsDestroyed() const;
        [[nodiscard]] bool hasShip(std::string const &type) const;
        [[nodiscard]] std::string toString(bool showShips) const;
        [[nodiscard]] std::string toString() const;

        [[nodiscard]] auto getHitPointAt(Coordinate const &coordinate);
        PlacementResult placeShip(Ship const &ship);
        HitResult fireAt(Coordinate const &coordinate);
    };

    typedef std::vector<Sea> PlayerBoards;
}

#endif //BATTLESHIP_SEA_H
