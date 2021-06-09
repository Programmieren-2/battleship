//
// Created by rne on 09.06.21.
//

template <typename ElementType>
bool contains(std::vector<ElementType> const &haystack, ElementType const &needle)
{
    return std::ranges::find(haystack, needle) != haystack.end();
}