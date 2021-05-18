//
// Created by rne on 07.05.21.
//

#ifndef BATTLESHIP_UTIL_H
#define BATTLESHIP_UTIL_H

#include <string>
#include <vector>

#include <boost/program_options.hpp>
using boost::program_options::options_description;
using boost::program_options::variables_map;

namespace util {
    std::vector<std::string> splitString(std::string const &str, std::string const &delimiter);
    std::vector<std::string> splitString(std::string const &str);
    std::string readWithPrompt(std::string const &prompt);
    std::vector<std::string> readCommandLine(std::string const &prompt);
    bool isExitCommand(std::string const &command);
    bool isNumber(std::string const &str);
    int copyString(char *dest, std::string const &src, size_t size);
    std::optional<variables_map> parseArgDesc(int argc, const char *argv[], options_description desc);

    template <typename ElementType>
    bool contains(std::vector<ElementType> const &haystack, ElementType const &needle)
    {
        return std::find(haystack.begin(), haystack.end(), needle) != haystack.end();
    }
}

#endif //BATTLESHIP_UTIL_H
