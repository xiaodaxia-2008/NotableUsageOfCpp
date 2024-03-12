#include "Base.h"

Base::~Base() {}

std::string Base::print() const { return fmt::format("{}", *this); }
