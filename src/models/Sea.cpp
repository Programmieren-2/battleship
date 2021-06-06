//
// Created by rne on 07.05.21.
//

#include <algorithm>
using std::ranges::all_of;
using std::ranges::any_of;

#include <functional>
using std::reference_wrapper;

#include <optional>
using std::optional;

#include <ostream>
using std::ostream;

#include <stdexcept>
using std::out_of_range;

#include <string>
using std::string;

#include <utility>
using std::move;

#include <vector>
using std::vector;

#include "exceptions.h"
#include "Defaults.h"
#include "Sea.h"
#include "Ship.h"

namespace models {
    Sea::Sea(unsigned short width, unsigned short height)
        : width(width), height(height), grid(Matrix<HitPoint>(width, height, HitPoint()))
    {}

    Sea::Sea()
        : Sea(Defaults::WIDTH, Defaults::HEIGHT)
    {}

    unsigned short Sea::getWidth() const
    {
        return width;
    }

    unsigned short Sea::getHeight() const
    {
        return height;
    }

    bool Sea::shipOnBoard(Ship const &ship) const
    {
        return all_of(ship.getCoordinates(), [this] (auto const &coordinate) {
            return coordinate.insideBounds(width, height);
        });
    }

    bool Sea::shipCollides(Ship const &ship) const
    {
        return any_of(ships, [ship] (Ship const &other) {
            return ship.collidesWith(other);
        });
    }

    char Sea::getSymbolAt(Coordinate const &coordinate, bool showShips) const
    {
        for (auto const &ship : ships) {
            if (ship.isHitAt(coordinate))
                return 'x';

            if (showShips && ship.occupies(coordinate))
                return '#';
        }

        return grid.at(coordinate).isHit() ? 'o' : '~';
    }

    bool Sea::allShipsDestroyed() const
    {
        return all_of(ships, [] (Ship const &ship) { return ship.isDestroyed(); });
    }

    bool Sea::hasShip(string const &type) const
    {
        return any_of(ships, [type] (Ship const &ship) { return ship.getType() == type; });
    }

    string Sea::toString(bool showShips) const
    {
        string result;

        for (unsigned short y = 0; y < getHeight(); ++y) {
            for (unsigned short x = 0; x < getWidth(); ++x)
                result += getSymbolAt(Coordinate(x, y), showShips);

            result += "\n";
        }

        return result;
    }

    string Sea::toString() const
    {
        return toString(false);
    }

    void Sea::placeShip(Ship const &ship)
    {
        if (!shipOnBoard(ship))
            throw OutsideSeaBounds(width, height);

        if (shipCollides(ship))
            throw Collision();

        ships.push_back(ship);
    }

    HitResult Sea::fireAt(Coordinate const &coordinate)
    {
        for (auto &ship : ships) {
            switch (ship.fireAt(coordinate)) {
                case ALREADY_HIT:
                    return ALREADY_HIT;
                case HIT:
                    return HIT;
                default:
                    continue;
            }
        }

        HitPoint hitPoint;

        try {
            hitPoint = grid.at(coordinate);
        } catch (out_of_range const &) {
            return MISSED;
        }

        switch (hitPoint.doHit()) {
            case ALREADY_HIT:
                return ALREADY_HIT;
            default:
                return MISSED;
        }
    }

    ostream& operator<<(std::ostream &out, const Sea &sea)
    {
        out << sea.toString();
        return out;
    }
}