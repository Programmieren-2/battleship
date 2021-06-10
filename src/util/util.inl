//
// Created by rne on 09.06.21.
//

template <template<typename, typename> class Container, typename ElementType>
bool contains(Container<ElementType, std::allocator<ElementType>> const &haystack, ElementType const &needle)
{
    return std::ranges::find(haystack, needle) != haystack.end();
}