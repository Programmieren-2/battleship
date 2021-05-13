//
// Created by rne on 07.05.21.
//

#ifndef BATTLESHIP_UTIL_H
#define BATTLESHIP_UTIL_H

#include <string>
#include <vector>

namespace util {
    std::vector <std::string> splitString(std::string const &str, std::string const &delimiter);
    std::string readWithPrompt(std::string const &prompt);
    bool isNumber(std::string const &str);
    int copyString(char *dest, std::string const &src, size_t size);

    template <typename ElementType>
    bool contains(std::vector<ElementType> const &haystack, ElementType const &needle)
    {
        return std::find(haystack.begin(), haystack.end(), needle) != haystack.end();
    }
}

#endif //BATTLESHIP_UTIL_H