#include "state_vector.hpp"
#include <sstream>
#include <frontend/rules/rules.hpp>

StateVector::operator std::string() const
{
    std::stringstream ss;
    ss << "[ ";
    for (Rules::Cases c: *this) {
        ss << to_string(c) << " ";
    }
    ss << "]";
    return ss.str();
}
