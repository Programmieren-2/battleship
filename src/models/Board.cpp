//
// Created by rne on 07.05.21.
//

#include <functional>
using std::reference_wrapper;

#include <optional>
using std::optional;

#include <stdexcept>
using std::out_of_range;

#include <boost/config.hpp>

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

    auto Board::getHitPointAt(Coordinate const &coordinate)
    {
        optional<reference_wrapper<HitPoint>> hitPoint;

        try {
            return hitPoint = grid.at(coordinate.getY()).at(coordinate.getX());
        } catch (out_of_range&) {
            return hitPoint;
        }
    }

    HitResult Board::fireAt(Coordinate const &coordinate)
    {
        auto candidate = getHitPointAt(coordinate);

        if (BOOST_UNLIKELY(!candidate.has_value()))
            return HitResult::MISSED;

        HitPoint &hitPoint = candidate.value();
        return hitPoint.doHit();
    }
}