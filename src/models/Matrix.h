//
// Created by rne on 04.06.21.
//

#pragma once

#include <vector>

#include "Coordinate.h"

namespace models {
    template <typename ElementType>
    class Matrix : public std::vector<std::vector<ElementType>> {
    public:
        using std::vector<std::vector<ElementType>>::at;

        Matrix(size_t width, size_t height, ElementType const &initializer)
        {
            if (!width || ! height)
                throw std::invalid_argument("Matrix size must be > 0.");

            for (size_t y = 0; y < height; ++y)
                this->push_back(std::vector<ElementType>(width, initializer));
        }

        ElementType const & at(Coordinate const &coordinate) const
        {
            return at(coordinate.second).at(coordinate.first);
        }

        ElementType& at(Coordinate const &coordinate)
        {
            return at(coordinate.second).at(coordinate.first);
        }
    };
}