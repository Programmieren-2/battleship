//
// Created by rne on 04.06.21.
//

#pragma once

#include <vector>

#include "Coordinate.h"

namespace models {
    template <typename T>
    class Matrix : public std::vector<std::vector<T>> {
    public:
        using std::vector<std::vector<T>>::at;

        Matrix(size_t width, size_t height, T const &initializer)
        {
            if (!width || ! height)
                throw std::invalid_argument("Matrix size must be > 0.");

            for (size_t y = 0; y < height; ++y)
                push_back(std::vector<T>(width, initializer));
        }

        T const & at(Coordinate const &coordinate) const {
            return at(coordinate.second).at(coordinate.first);
        }

        T& at(Coordinate const &coordinate) {
            return at(coordinate.second).at(coordinate.first);
        }
    };
}