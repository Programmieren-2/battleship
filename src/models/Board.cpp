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
}