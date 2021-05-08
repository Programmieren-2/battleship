//
// Created by rne on 07.05.21.
//
#include "Board.h"
#include "Coordinate.h"

namespace models {
    Board::Board(unsigned short width, unsigned short height)
        : width(width), height(height)
    {}

    unsigned short Board::getWidth() const
    {
        return width;
    }

    unsigned short Board::getHeight() const
    {
        return height;
    }

    Coordinates Board::getCoordinates() const
    {
        Coordinates coordinates;

        for (unsigned short x = 0; x < width; x++) {
            for (unsigned short y = 0; y < height; y++)
                coordinates.push_back(Coordinate(x, y));
        }

        return coordinates;
    }
}