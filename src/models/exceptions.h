//
// Created by rne on 05.06.21.
//

#pragma once

#include <exception>
#include <string>

namespace models {
    class PlaceShipError : public std::exception {
    public:
        [[nodiscard]] char const *what() const noexcept override = 0;
    };

    class OutsideSeaBounds : public PlaceShipError {
    private:
        std::string message;
    public:
        OutsideSeaBounds(unsigned short width, unsigned short height);

        [[nodiscard]] char const *what() const noexcept override;
    };

    class Collision : public PlaceShipError {
    public:
        [[nodiscard]] char const *what() const noexcept override;
    };
}