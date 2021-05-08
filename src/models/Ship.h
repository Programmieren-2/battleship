//
// Created by rne on 07.05.21.
//

#ifndef BATTLESHIP_SHIP_H
#define BATTLESHIP_SHIP_H

#include <map>
#include <string>
#include <vector>

#include "Coordinate.h"
#include "HitPoint.h"

namespace models {
    class Ship {
    private:
        std::string type;
        Coordinate anchorPoint;
        unsigned short length;
        Orientation orientation;
        HitPoints hitPoints;
    public:
        Ship(std::string type, Coordinate const &anchorPoint, unsigned short length, Orientation orientation);

        Coordinate getAnchorPoint() const;
        Coordinate getEndPoint() const;
        HitPoints getHitPoints() const;
        bool collidesWith(Ship const &ship) const;
        bool isDestroyed() const;
        std::string getType() const;

        HitResult hitAt(Coordinate const &coordinate);

        bool operator==(Ship const &other) const;
        bool operator!=(Ship const &other) const;
    };

    typedef std::vector<Ship> Ships;
    typedef std::map<std::string, int> ShipTypes;
}

#endif //BATTLESHIP_SHIP_H
