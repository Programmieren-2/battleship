//
// Created by rne on 07.05.21.
//

#include "Board.h"
#include "Constants.h"
#include "PlayerBoard.h"
#include "GameBoard.h"

namespace models {
    GameBoard::GameBoard(unsigned int width, unsigned int height)
            : Board(width, height), playerBoards{PlayerBoard(width, height, {}), PlayerBoard(width, height, {})}
    {}

    GameBoard::GameBoard()
            : GameBoard(Constants::width, Constants::height)
    {}

    PlayerBoards GameBoard::getPlayerBoards() const
    {
        return playerBoards;
    }
}