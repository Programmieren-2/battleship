//
// Created by rne on 07.05.21.
//

#ifndef BATTLESHIP_COORDINATE_H
#define BATTLESHIP_COORDINATE_H

#include <string>

namespace models {
    enum Orientation { X, Y };

    class Coordinate {
    private:
        unsigned short x;
        unsigned short y;
    public:
        Coordinate(const Coordinate&) = default;
        Coordinate(unsigned short x, unsigned short y);

        [[nodiscard]] unsigned short getX() const;
        [[nodiscard]] unsigned short getY() const;

        bool operator==(Coordinate const &coordinate) const;
    };
}


#endif //BATTLESHIP_COORDINATE_H
