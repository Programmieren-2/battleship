//
// Created by rne on 07.05.21.
//

#pragma once

#include <optional>
#include <string>
#include <utility>

namespace models {
    enum Orientation : unsigned char { X, Y };

    class Coordinate : public std::pair<uint8_t, uint8_t> {
    public:
        Coordinate(uint8_t x, uint8_t y);

        [[nodiscard]] Coordinate shift(unsigned short offset, Orientation orientation) const;
        [[nodiscard]] bool insideBounds(unsigned short x, unsigned short y) const;

        static std::optional<Coordinate> fromString(std::string const &strX, std::string const &strY);
    };
}