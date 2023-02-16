#include "zpp_bits.h"

#include <boost/pfr.hpp>
#include <fmt/format.h>
#include <fmt/ranges.h>

#include <iostream>
#include <string>
#define IC(...) fmt::print(#__VA_ARGS__ ":|{}|\n", __VA_ARGS__)

namespace pfr = boost::pfr;

struct Person
{
    // friend struct zpp::bits::access;
    // using serialize = zpp::bits::members<2>;

    int age{13};
    std::string name{"Zen"};
};

int main()
{
    auto [data, in, out] = zpp::bits::data_in_out();

    Person person;
    std::cout << pfr::io(person) << std::endl;

    out(person).or_throw();
    IC(data);

    Person person1;
    in(person1).or_throw();
    std::cout << pfr::io(person) << std::endl;

    return 0;
}