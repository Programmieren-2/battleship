//
// Created by rne on 07.05.21.
//

#ifndef BATTLESHIP_GAMEBOARD_H
#define BATTLESHIP_GAMEBOARD_H
#include "PlayerBoard.h"

namespace models {
    class GameBoard : public Board {
    private:
        PlayerBoards playerBoards;
    public:
        GameBoard(unsigned int width, unsigned int height);
        GameBoard();

        PlayerBoards getPlayerBoards() const;
    };
}

#endif //BATTLESHIP_GAMEBOARD_H
