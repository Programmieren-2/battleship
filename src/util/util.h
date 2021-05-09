//
// Created by rne on 07.05.21.
//

#ifndef BATTLESHIP_UTIL_H
#define BATTLESHIP_UTIL_H

#include <algorithm>
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

    template <typename ElementType>
    bool contains(std::vector<ElementType> const &haystack, ElementType const &needle)
    {
        return *std::find(haystack.begin(), haystack.end(), needle) == needle;
    }
}

#endif //BATTLESHIP_UTIL_H
