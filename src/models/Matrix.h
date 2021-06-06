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

        T const & at(Coordinate const &coordinate) const {
            return at(coordinate.second).at(coordinate.first);
        }

        T& at(Coordinate const &coordinate) {
            return at(coordinate.second).at(coordinate.first);
        }
    };
}