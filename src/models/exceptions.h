//
// Created by rne on 05.06.21.
//

#pragma once

#include <exception>
#include <string>

namespace models {
    class PlaceShipError : public std::exception {};

    class OutsideSeaBounds : public PlaceShipError {
    private:
        std::string message;
    public:
        OutsideSeaBounds(unsigned short width, unsigned short height);

        char const *what() const noexcept override;
    };

    class Collision : public PlaceShipError {
    public:
        char const *what() const noexcept override;
    };
}