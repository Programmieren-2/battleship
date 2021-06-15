//
// Created by rne on 16.06.21.
//

template <typename ElementType>
Matrix<ElementType>::Matrix(size_t width, size_t height, ElementType const &initializer)
{
    if (!width || !height)
        throw std::invalid_argument("Matrix size must be > 0.");

    for (size_t y = 0; y < height; ++y)
        push_back(std::vector<ElementType>(width, initializer));
}

template <typename ElementType>
ElementType const & Matrix<ElementType>::at(Coordinate const &coordinate) const
{
    return at(coordinate.second).at(coordinate.first);
}

template <typename ElementType>
ElementType& Matrix<ElementType>::at(Coordinate const &coordinate)
{
    return at(coordinate.second).at(coordinate.first);
}
