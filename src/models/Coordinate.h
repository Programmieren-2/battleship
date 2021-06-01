//
// Created by rne on 07.05.21.
//

#pragma once

#include <optional>
#include <string>

namespace models {
    enum Orientation : unsigned char { X, Y };

    class Coordinate {
    private:
        unsigned short x;
        unsigned short y;
    public:
        Coordinate(const Coordinate&) = default;
        Coordinate(unsigned short x, unsigned short y);

        [[nodiscard]] unsigned short getX() const;
        [[nodiscard]] unsigned short getY() const;

        static std::optional<Coordinate> fromString(std::string const &strX, std::string const &strY);

        bool operator==(Coordinate const &coordinate) const;
    };
}