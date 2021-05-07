//
// Created by rne on 07.05.21.
//

#include <iostream>
using std::cout;
using std::endl;

#include "Board.h"
#include "PlayerBoard.h"
#include "Ship.h"

namespace models {
    PlayerBoard::PlayerBoard(unsigned short width, unsigned short height, Ships ships)
            : Board(width, height), ships(ships)
    {}

    bool PlayerBoard::coordinateOnBoard(const Coordinate &coordinate) const
    {
        return coordinate.getX() < width && coordinate.getY() < height;
    }

    bool PlayerBoard::shipOnBoard(const Ship &ship) const
    {
        return coordinateOnBoard(ship.getAnchorPoint()) && coordinateOnBoard(ship.getEndPoint());
    }

    bool PlayerBoard::shipCollides(const Ship &ship) const
    {
        for (Ship existingShip : ships) {
            if (existingShip.collidesWith(ship))
                return true;
        }

        return false;
    }

    PlacementResult PlayerBoard::checkPlacement(const Ship &ship) const
    {
        if (!shipOnBoard(ship))
            return PlacementResult::NOT_ON_BOARD;

        if (shipCollides(ship))
            return PlacementResult::COLLISION;

        return PlacementResult::SUCCESS;
    }

    bool PlayerBoard::placeShip(const Ship &ship)
    {
        switch (checkPlacement(ship)) {
            case PlacementResult::NOT_ON_BOARD:
                cout << "Ship " + ship.toString() + " is not on the board." << endl;
                return false;
            case PlacementResult::COLLISION:
                cout << "Ship " + ship.toString() + " collides with another ship." << endl;
                return false;
            case PlacementResult::SUCCESS:
                return true;
        }

        return false;
    }

    bool PlayerBoard::fireAt(const Coordinate &coordinate)
    {
        for (Ship ship : ships) {
            if (ship.hitAt(coordinate))
                return true;
        }

        return false;
    }
}