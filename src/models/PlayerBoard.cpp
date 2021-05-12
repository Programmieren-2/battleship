//
// Created by rne on 07.05.21.
//

#include <algorithm>
using std::all_of;
using std::any_of;

#include <string>
using std::string;

#include "Board.h"
#include "Constants.h"
#include "PlayerBoard.h"
#include "Ship.h"

namespace models {
    PlayerBoard::PlayerBoard(string const &name, unsigned short width, unsigned short height)
            : Board(width, height), name(name), ships(Ships()), misses(Coordinates())
    {}

    PlayerBoard::PlayerBoard(string const &name)
            : PlayerBoard(name, Constants::WIDTH, Constants::HEIGHT)
    {}

    string PlayerBoard::getName() const
    {
        return name;
    }

    bool PlayerBoard::coordinateOnBoard(Coordinate const &coordinate) const
    {
        return coordinate.getX() < getWidth() && coordinate.getY() < getHeight();
    }

    bool PlayerBoard::shipOnBoard(Ship const &ship) const
    {
        return coordinateOnBoard(ship.getAnchorPoint()) && coordinateOnBoard(ship.getEndPoint());
    }

    bool PlayerBoard::shipCollides(Ship const &ship) const
    {
        return any_of(ships.begin(), ships.end(), [ship](Ship const &other) {
            return ship.collidesWith(other);
        });
    }

    string PlayerBoard::getSymbolAt(Coordinate const &coordinate, bool showShips) const
    {
        for (Ship const &ship : ships) {
            for (HitPoint const &hitPoint : ship.getHitPoints()) {
                if (hitPoint == coordinate) {
                    if (hitPoint.isHit())
                        return "x";

                    if (showShips)
                        return "#";
                }
            }
        }

        for (Coordinate miss : misses) {
            if (miss == coordinate)
                return "o";
        }

        return "~";
    }

    bool PlayerBoard::allShipsDestroyed() const
    {
        return all_of(ships.begin(), ships.end(), [](Ship const &ship){ return ship.isDestroyed(); });
    }

    string PlayerBoard::toString(bool showShips) const
    {
        string result;

        for (unsigned short y = 0; y < getHeight(); y++) {
            for (unsigned short x = 0; x < getWidth(); x++)
                result += getSymbolAt(Coordinate(x, y), showShips);

            result += "\n";
        }

        return result;
    }

    string PlayerBoard::toString() const
    {
        return toString(false);
    }

    PlacementResult PlayerBoard::placeShip(Ship const &ship)
    {
        if (!shipOnBoard(ship))
            return PlacementResult::NOT_ON_BOARD;

        if (shipCollides(ship))
            return PlacementResult::COLLISION;

        ships.push_back(ship);
        return PlacementResult::SUCCESS;
    }

    HitResult PlayerBoard::fireAt(Coordinate const &coordinate)
    {
        for (Ship &ship : ships) {
            switch (ship.hitAt(coordinate)) {
                case HitResult::MISSED:
                    continue;
                case HitResult::ALREADY_HIT:
                    return HitResult::ALREADY_HIT;
                case HitResult::HIT:
                    return HitResult::HIT;
            }
        }

        misses.push_back(coordinate);
        return HitResult::MISSED;
    }
}