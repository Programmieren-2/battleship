//
// Created by rne on 07.05.21.
//

#ifndef BATTLESHIP_UTIL_H
#define BATTLESHIP_UTIL_H

#include <string>
#include <vector>

#include "Coordinate.h"
#include "PlayerBoard.h"
#include "Ship.h"

namespace util {
    std::vector <std::string> splitString(std::string const &str, std::string const &delimiter);
    std::string readWithPrompt(std::string const &prompt);
}

#endif //BATTLESHIP_UTIL_H
