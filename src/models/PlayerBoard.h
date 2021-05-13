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

        [[nodiscard]] bool coordinateOnBoard(Coordinate const &coordinate) const;
        [[nodiscard]] bool shipOnBoard(Ship const &ship) const;
        [[nodiscard]] bool shipCollides(Ship const &ship) const;
        [[nodiscard]] std::string getSymbolAt(Coordinate const &coordinate, bool showShips) const;
    public:
        PlayerBoard(std::string name, unsigned short width, unsigned short height);
        explicit PlayerBoard(std::string const &name);

        [[nodiscard]] std::string getName() const;
        [[nodiscard]] bool allShipsDestroyed() const;
        [[nodiscard]] std::string toString(bool showShips) const;
        [[nodiscard]] std::string toString() const;

        PlacementResult placeShip(Ship const &ship);
        HitResult fireAt(Coordinate const &coordinate) override;
    };

    typedef std::vector<PlayerBoard> PlayerBoards;
}

#endif //BATTLESHIP_PLAYERBOARD_H
