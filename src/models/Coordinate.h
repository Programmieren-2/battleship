//
// Created by rne on 07.05.21.
//

#pragma once

#include <optional>
#include <string>
#include <utility>

namespace models {
    enum Orientation : unsigned char { X, Y };

    class Coordinate : public std::pair<unsigned short, unsigned short> {
    public:
        Coordinate(unsigned short x, unsigned short y);

        [[nodiscard]] Coordinate shift(unsigned short offset, Orientation const &orientation) const;
        [[nodiscard]] bool insideBounds(unsigned short x, unsigned short y) const;

        static std::optional<Coordinate> fromString(std::string const &strX, std::string const &strY);
    };
}