//
// Created by rne on 07.05.21.
//

#include <algorithm>
using std::all_of;
using std::any_of;

#include <functional>
using std::reference_wrapper;

#include <optional>
using std::optional;

#include <stdexcept>
using std::out_of_range;

#include <string>
using std::string;

#include <utility>
using std::move;

#include "Models.h"
#include "Sea.h"
#include "Ship.h"

namespace models {
    Sea::Sea(unsigned short width, unsigned short height)
            : width(width), height(height), ships(Ships())
    {
        initializeGrid();
    }

    Sea::Sea()
            : Sea(Defaults::WIDTH, Defaults::HEIGHT)
    {}

    void Sea::initializeGrid()
    {
        for (unsigned short y = 0; y < height; y++) {
            HitPoints row;

            for (unsigned short x = 0; x < height; x++)
                row.push_back(HitPoint(x, y));

            grid.push_back(row);
        }
    }

    unsigned short Sea::getWidth() const
    {
        return width;
    }

    unsigned short Sea::getHeight() const
    {
        return height;
    }

    auto Sea::getHitPointAt(Coordinate const &coordinate)
    {
        optional<reference_wrapper<HitPoint>> hitPoint;

        try {
            return hitPoint = grid.at(coordinate.getY()).at(coordinate.getX());
        } catch (out_of_range&) {
            return hitPoint;
        }
    }

    bool Sea::coordinateOnBoard(Coordinate const &coordinate) const
    {
        return coordinate.getX() < getWidth() && coordinate.getY() < getHeight();
    }

    bool Sea::shipOnBoard(Ship const &ship) const
    {
        return coordinateOnBoard(ship.getAnchorPoint()) && coordinateOnBoard(ship.getEndPoint());
    }

    bool Sea::shipCollides(Ship const &ship) const
    {
        return any_of(ships.begin(), ships.end(), [ship](Ship const &other) {
            return ship.collidesWith(other);
        });
    }

    string Sea::getSymbolAt(Coordinate const &coordinate, bool showShips) const
    {
        for (Ship const &ship : ships) {
            if (ship.isHitAt(coordinate))
                return "x";

            if (showShips && ship.occupies(coordinate))
                return "#";
        }

        HitPoint hitPoint = grid.at(coordinate.getY()).at(coordinate.getX());
        return hitPoint.isHit() ? "o" : "~";
    }

    bool Sea::allShipsDestroyed() const
    {
        return all_of(ships.begin(), ships.end(), [](Ship const &ship){ return ship.isDestroyed(); });
    }

    bool Sea::hasShip(std::string const &type) const
    {
        return any_of(ships.begin(), ships.end(), [type](Ship const &ship){ return ship.getType() == type; });
    }

    string Sea::toString(bool showShips) const
    {
        string result;

        for (unsigned short y = 0; y < getHeight(); y++) {
            for (unsigned short x = 0; x < getWidth(); x++)
                result += getSymbolAt(Coordinate(x, y), showShips);

            result += "\n";
        }

        return result;
    }

    string Sea::toString() const
    {
        return toString(false);
    }

    PlacementResult Sea::placeShip(Ship const &ship)
    {
        if (!shipOnBoard(ship))
            return PlacementResult::NOT_ON_BOARD;

        if (shipCollides(ship))
            return PlacementResult::COLLISION;

        ships.push_back(ship);
        return PlacementResult::SUCCESS;
    }

    HitResult Sea::fireAt(Coordinate const &coordinate) {
        for (Ship &ship : ships) {
            switch (ship.fireAt(coordinate)) {
                case HitResult::ALREADY_HIT:
                    return HitResult::ALREADY_HIT;
                case HitResult::HIT:
                    return HitResult::HIT;
                default:
                    continue;
            }
        }

        auto candidate = getHitPointAt(coordinate);

        if (!candidate.has_value())
            return HitResult::MISSED;

        HitPoint &hitPoint = candidate.value();

        switch (hitPoint.doHit()) {
            case HitResult::ALREADY_HIT:
                return HitResult::ALREADY_HIT;
            default:
                return HitResult::MISSED;
        }
    }
}