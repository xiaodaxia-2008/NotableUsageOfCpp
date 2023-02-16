#include <array>
#include <iostream>
#include <iterator>
#include <string_view>
#include <type_traits>

using namespace std::literals;

template <typename T> struct MyVec
{
    template <typename Iter> MyVec(Iter, Iter)
    {
    }
};

// “deduction guides” that tell the compiler how to deduce class template arguments from constructor arguments
template <typename Iter> MyVec(Iter, Iter) -> MyVec<typename std::iterator_traits<Iter>::value_type>;

template <typename A, typename B> struct MyAdvancedPair
{
    template <typename T, typename U> MyAdvancedPair(T &&, U &&)
    {
    }
};
// deduction guides
template <typename X, typename Y> MyAdvancedPair(X, Y) -> MyAdvancedPair<X, Y>;

int main()
{
    std::array arr = {"hello"sv, "world"sv};
    static_assert(std::is_same_v<decltype(arr), std::array<std::string_view, 2>>);

    int *ptr = nullptr;
    MyVec v(ptr, ptr);
    static_assert(std::is_same_v<decltype(v), MyVec<int>>);
    MyAdvancedPair adv(1729, "taxicab");
    static_assert(std::is_same_v<decltype(adv), MyAdvancedPair<int, const char *>>);
    return 0;
}