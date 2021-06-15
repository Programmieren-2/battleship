//
// Created by rne on 04.06.21.
//

#pragma once

#include <vector>

#include "Coordinate.h"

namespace models {
    template <typename ElementType>
    class Matrix : public std::vector<std::vector<ElementType>> {
    private:
        using std::vector<std::vector<ElementType>>::at;
        using std::vector<std::vector<ElementType>>::push_back;
    public:
        Matrix(size_t width, size_t height, ElementType const &initializer);

        ElementType const & at(Coordinate const &coordinate) const;
        ElementType& at(Coordinate const &coordinate);
    };

#include "Matrix.inl"
}