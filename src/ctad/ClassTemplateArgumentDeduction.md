# Class Template Argument Deduction (模板参数推导)

## Template Argument Deduction

C++98 through C++14 performed template argument deduction for function templates. Given a function template like template <typename RanIt> void sort(RanIt first, RanIt last);, you can and should sort a std::vector<int> without explicitly specifying that RanIt is std::vector<int>::iterator. When the compiler sees sort(v.begin(), v.end());, it knows what the types of v.begin() and v.end() are, so it can determine what RanIt should be. The process of determining template arguments for template parameters (by comparing the types of function arguments to function parameters, according to rules in the Standard) is known as template argument deduction, which makes function templates far more usable than they would otherwise be.

However, class templates didn’t benefit from these rules. If you wanted to construct a std::pair from two ints, you had to say std::pair<int, int> p(11, 22);, despite the fact that the compiler already knows that the types of 11 and 22 are int. The workaround for this limitation was to use function template argument deduction: std::make_pair(11, 22) returns std::pair<int, int>. Like most workarounds, this is problematic for a few reasons: defining such helper functions often involves template metaprogramming (std::make_pair() needs to perform perfect forwarding and decay, among other things), compiler throughput is reduced (as the front-end has to instantiate the helper, and the back-end has to optimize it away), debugging is more annoying (as you have to step through helper functions), and there’s still a verbosity cost (the extra make_ prefix, and if you want a local variable instead of a temporary, you need to say auto).

```c++
#include <type_traits>
#include <utility>
int main() {
    std::pair p(1729, "taxicab");
    static_assert(std::is_same_v<decltype(p), std::pair<int, const char *>>);
}
```

## Deduction Guides

In general, CTAD automatically works when class templates have constructors whose signatures mention all of the class template parameters (like MyPair above). However, sometimes constructors themselves are templated, which breaks the connection that CTAD relies on. In those cases, the author of the class template can provide “deduction guides” that tell the compiler how to deduce class template arguments from constructor arguments.

```c++
#include <iterator>
#include <type_traits>
template <typename T> struct MyVec {
    template <typename Iter> MyVec(Iter, Iter) { }
};
template <typename Iter> MyVec(Iter, Iter) -> MyVec<typename std::iterator_traits<Iter>::value_type>;
template <typename A, typename B> struct MyAdvancedPair {
    template <typename T, typename U> MyAdvancedPair(T&&, U&&) { }
};
template <typename X, typename Y> MyAdvancedPair(X, Y) -> MyAdvancedPair<X, Y>;
int main() {
    int * ptr = nullptr;
    MyVec v(ptr, ptr);
    static_assert(std::is_same_v<decltype(v), MyVec<int>>);
    MyAdvancedPair adv(1729, "taxicab");
    static_assert(std::is_same_v<decltype(adv), MyAdvancedPair<int, const char *>>);
}
```

[reference](https://devblogs.microsoft.com/cppblog/how-to-use-class-template-argument-deduction/)