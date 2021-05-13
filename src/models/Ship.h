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

        void initializeGrid();
    public:
        Ship(std::string type, Coordinate const &anchorPoint, unsigned short length, Orientation orientation);

        [[nodiscard]] Coordinate getAnchorPoint() const;
        [[nodiscard]] Coordinate getEndPoint() const;
        [[nodiscard]] std::string getType() const;
        [[nodiscard]] HitPoints getHitPoints() const;
        [[nodiscard]] bool occupies(Coordinate const &coordinate) const;
        [[nodiscard]] bool collidesWith(Ship const &ship) const;
        [[nodiscard]] bool isHitAt(Coordinate const &coordinate) const;
        [[nodiscard]] bool isDestroyed() const;

        HitResult fireAt(Coordinate const &coordinate);

        bool operator==(Ship const &other) const;
        bool operator!=(Ship const &other) const;
    };

    typedef std::vector<Ship> Ships;
    typedef std::map<std::string, unsigned short> ShipTypes;
}

#endif //BATTLESHIP_SHIP_H
