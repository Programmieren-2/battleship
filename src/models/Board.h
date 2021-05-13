//
// Created by rne on 07.05.21.
//

#ifndef BATTLESHIP_BOARD_H
#define BATTLESHIP_BOARD_H

#include <optional>

#include "Coordinate.h"
#include "HitPoint.h"

namespace models {
    class Board {
    private:
        unsigned short width;
        unsigned short height;

        void initializeGrid();
    protected:
        Grid grid;
    public:
        Board(unsigned short width, unsigned short height);

        [[nodiscard]] unsigned short getWidth() const;
        [[nodiscard]] unsigned short getHeight() const;

        [[nodiscard]] std::optional<HitPoint> getHitPointAt(Coordinate const &coordinate);
        [[nodiscard]] virtual HitResult fireAt(Coordinate const &coordinate);
    };
}


#endif //BATTLESHIP_BOARD_H
