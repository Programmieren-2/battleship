//
// Created by rne on 07.05.21.
//

#include <math.h>
#include <string>
using std::string;
using std::to_string;

#include "Coordinate.h"

namespace models {
    static int distance(unsigned short a, unsigned short b)
    {
        return (a > b) ? a - b : b - a;
    }

    Coordinate::Coordinate(unsigned short x, unsigned short y)
            : x(x), y(y)
    {}

    unsigned short Coordinate::getX() const
    {
        return x;
    }

    unsigned short Coordinate::getY() const
    {
        return y;
    }

    double Coordinate::distanceTo(const Coordinate &coordinate) const
    {
        return sqrt(pow(distance(x, coordinate.x), 2) + pow(distance(y, coordinate.y), 2));
    }

    string Coordinate::toString() const
    {
        return to_string(x) + "x" + to_string(y);
    }

    bool Coordinate::operator==(const Coordinate &coordinate) const
    {
        return x == coordinate.x && y == coordinate.y;
    }
}