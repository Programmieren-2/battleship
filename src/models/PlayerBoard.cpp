//
// Created by rne on 07.05.21.
//

#include <string>
using std::string;

#include "Board.h"
#include "Constants.h"
#include "PlayerBoard.h"
#include "Ship.h"

namespace models {
    PlayerBoard::PlayerBoard(string name, unsigned short width, unsigned short height)
            : Board(width, height), name(name), ships(Ships()), misses(Coordinates())
    {}

    PlayerBoard::PlayerBoard(std::string name)
            : PlayerBoard(name, Constants::width, Constants::height)
    {}

    string PlayerBoard::getName() const
    {
        return name;
    }

    bool PlayerBoard::coordinateOnBoard(const Coordinate &coordinate) const
    {
        return coordinate.getX() < getWidth() && coordinate.getY() < getHeight();
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

    string PlayerBoard::getCharAt(const Coordinate &coordinate, bool showShips) const
    {
        for (Ship ship : ships) {
            for (HitPoint hitPoint : ship.getHitPoints()) {
                if (hitPoint == coordinate) {
                    if (hitPoint.isHit())
                        return "x";

                    if (showShips)
                        return "█";
                }
            }
        }

        for (Coordinate miss : misses) {
            if (miss == coordinate)
                return "o";
        }

        return "~";
    }

    PlacementResult PlayerBoard::placeShip(const Ship &ship)
    {
        if (!shipOnBoard(ship))
            return PlacementResult::NOT_ON_BOARD;

        if (shipCollides(ship))
            return PlacementResult::COLLISION;

        ships.push_back(ship);
        return PlacementResult::SUCCESS;
    }

    HitResult PlayerBoard::fireAt(const Coordinate &coordinate)
    {
        for (Ship &ship : ships) {
            switch (ship.hitAt(coordinate)) {
                case HitResult::MISSED:
                    break;
                case HitResult::ALREADY_HIT:
                    return HitResult::ALREADY_HIT;
                case HitResult::HIT:
                    return HitResult::HIT;
            }
        }

        misses.push_back(coordinate);
        return HitResult::MISSED;
    }

    bool PlayerBoard::allShipsDestroyed() const
    {
        for (Ship ship : ships) {
            if (!ship.isDestroyed())
                return false;
        }

        return true;
    }

    string PlayerBoard::toString(bool showShips) const
    {
        string result = "";

        for (unsigned int y = 0; y < getHeight(); y++) {
            for (unsigned int x = 0; x < getWidth(); x++) {
                result += getCharAt(Coordinate(x, y), showShips);
            }

            result += "\n";
        }

        return result;
    }

    string PlayerBoard::toString() const
    {
        return toString(false);
    }
}