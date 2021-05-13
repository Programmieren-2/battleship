//
// Created by rne on 07.05.21.
//

#include <optional>
using std::optional;

#include <stdexcept>
using std::out_of_range;

#include "Coordinate.h"
#include "HitPoint.h"

#include "Board.h"

namespace models {
    Board::Board(unsigned short width, unsigned short height)
        : width(width), height(height), grid(Grid())
    {
        initializeGrid();
    }

    void Board::initializeGrid()
    {
        for (unsigned short y = 0; y < height; y++) {
            HitPoints row;

            for (unsigned short x = 0; x < height; x++)
                row.push_back(HitPoint(x, y));

            grid.push_back(row);
        }
    }

    unsigned short Board::getWidth() const
    {
        return width;
    }

    unsigned short Board::getHeight() const
    {
        return height;
    }

    optional<HitPoint> Board::getHitPointAt(Coordinate const &coordinate)
    {
        optional<HitPoint> hitPoint;
        optional<HitPoints> row;

        try {
            row = grid.at(coordinate.getY());
        } catch (out_of_range&) {
            return hitPoint;
        }

        try {
            hitPoint = row.value().at(coordinate.getX());
        } catch (out_of_range&) {
            return hitPoint;
        }

        return hitPoint;
    }

    HitResult Board::fireAt(Coordinate const &coordinate)
    {
        optional<HitPoint> hitPoint = getHitPointAt(coordinate);

        if (!hitPoint.has_value())
            return HitResult::MISSED;

        return hitPoint.value().doHit();
    }
}