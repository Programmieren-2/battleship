//
// Created by rne on 07.05.21.
//

#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include <boost/program_options.hpp>

namespace util {
    std::vector<std::string> splitString(std::string const &str, std::string const &delimiter);
    std::vector<std::string> splitString(std::string const &str);
    std::string joinStrings(std::vector<std::string> const &strings, std::string const &delimiter);
    std::string joinStrings(std::vector<std::string> const &strings);
    std::string readWithPrompt(std::string const &prompt);
    std::vector<std::string> readCommandLine(std::string const &prompt);
    bool isExitCommand(std::string const &command);
    bool isNumber(std::string const &str);
    int copyString(char *dest, std::string const &src, size_t size);
    std::optional<boost::program_options::variables_map>
        parseArgDesc(int argc, char const *argv[], boost::program_options::options_description const &desc);

    template <template<typename, typename> class Container, typename ElementType>
    bool contains(Container<ElementType, std::allocator<ElementType>> const &haystack, ElementType const &needle);

#include "util.inl"
}
