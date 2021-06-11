//
// Created by rne on 08.05.21.
//

#pragma once

#include <string>

#include "Coordinate.h"
#include "Game.h"
#include "Sea.h"

#include "util.h"

namespace models {
    Coordinate readCoordinate(std::string const &prompt);
    Coordinate readCoordinate();
    Orientation readOrientation(string const &prompt);
    Orientation readOrientation();
    Sea readSea(unsigned short width, unsigned short height);

    template <typename PlayerType>
    PlayerType readPlayer(unsigned short width, unsigned short height);

    template <typename PlayerType>
    Game<PlayerType> readGame();

#include "bootstrap.inl"
}