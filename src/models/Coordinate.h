//
// Created by rne on 07.05.21.
//

#ifndef BATTLESHIP_COORDINATE_H
#define BATTLESHIP_COORDINATE_H

#include <string>
#include <vector>

namespace models {
    class Coordinate {
    private:
        unsigned short x;
        unsigned short y;
    public:
        Coordinate(unsigned short x, unsigned short y);

        unsigned short getX() const;
        unsigned short getY() const;
        double distanceTo(const Coordinate &coordinate) const;
        std::string toString() const;

        bool operator==(Coordinate const & coordinate) const;
    };

    typedef std::vector<Coordinate> Coordinates;

    enum Orientation { X, Y };
}


#endif //BATTLESHIP_COORDINATE_H
