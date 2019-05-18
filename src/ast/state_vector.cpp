#include "state_vector.hpp"
#include "frontend/rules/rules.hpp"
#include <algorithm>
#include <sstream>

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

bool StateVector::contains(const Rules::Cases &val) const
{
    return std::find(begin(), end(), val) != end();
}
