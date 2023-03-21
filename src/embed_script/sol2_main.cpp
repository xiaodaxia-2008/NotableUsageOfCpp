// C++ code
#include <iostream>
#include <sol/sol.hpp>

struct Foo
{
    int x = 0;
    Foo(int x) : x(x)
    {
    }
};

int main()
{
    sol::state lua;
    lua.open_libraries(sol::lib::base);

    // register Foo class and its constructor
    lua.new_usertype<Foo>("Foo", sol::constructors<Foo(int)>(), "x", &Foo::x);

    // create a Foo object in C++
    Foo foo(42);

    // pass it to Lua as a reference
    lua["foo"] = std::ref(foo);

    // call Lua code to modify foo's x value
    lua.script("foo.x = foo.x + 1");

    // print foo's x value in C++
    std::cout << "x = " << foo.x << "\n"; // prints "x = 43"
}