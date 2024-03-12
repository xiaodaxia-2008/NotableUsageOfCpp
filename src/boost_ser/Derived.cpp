#include "Derived.h"

Derived::~Derived() {}

std::string Derived::print() const { return fmt::format("{}", *this); }
