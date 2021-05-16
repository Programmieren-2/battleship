//
// Created by rne on 07.05.21.
//

#ifndef BATTLESHIP_SEA_H
#define BATTLESHIP_SEA_H

#include <string>
#include <vector>

#include "Coordinate.h"
#include "Models.h"
#include "Ship.h"

namespace models {
    class Sea {
    private:
        unsigned short width;
        unsigned short height;
        Matrix<HitPoint> grid;
        std::vector<Ship> ships;

        void initializeGrid();
        [[nodiscard]] bool coordinateOnBoard(Coordinate const &coordinate) const;
        [[nodiscard]] bool shipOnBoard(Ship const &ship) const;
        [[nodiscard]] bool shipCollides(Ship const &ship) const;
        [[nodiscard]] char getSymbolAt(Coordinate const &coordinate, bool showShips) const;
    public:
        Sea(unsigned short width, unsigned short height);
        Sea();

        [[nodiscard]] unsigned short getWidth() const;
        [[nodiscard]] unsigned short getHeight() const;
        [[nodiscard]] bool allShipsDestroyed() const;
        [[nodiscard]] bool hasShip(std::string const &type) const;
        [[nodiscard]] std::string toString(bool showShips) const;
        [[nodiscard]] std::string toString() const;
        [[nodiscard]] auto getHitPointAt(Coordinate const &coordinate) const;

        [[nodiscard]] auto getHitPointAt(Coordinate const &coordinate);
        PlacementResult placeShip(Ship const &ship);
        HitResult fireAt(Coordinate const &coordinate);
    };
}

#endif //BATTLESHIP_SEA_H
