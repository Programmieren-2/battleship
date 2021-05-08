//
// Created by rne on 07.05.21.
//

#ifndef BATTLESHIP_UTIL_H
#define BATTLESHIP_UTIL_H

#include <regex>
#include <string>
#include <vector>

#include "Coordinate.h"
#include "PlayerBoard.h"
#include "Ship.h"

namespace util {
    std::vector <std::string> splitString(std::string const &str, std::string const &delimiter);
    std::string readWithPrompt(std::string const &prompt);
    bool isNumber(std::string const &str);

    models::Coordinate readCoordinate(std::string const &prompt);
    models::Orientation readOrientation(string const &prompt);
    models::PlayerBoard readPlayerBoard();
}

#endif //BATTLESHIP_UTIL_H
