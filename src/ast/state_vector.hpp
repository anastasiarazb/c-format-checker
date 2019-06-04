#ifndef C_FORMAT_CHECKER_STATE_VECTOR_HPP
#define C_FORMAT_CHECKER_STATE_VECTOR_HPP

#include <vector>
#include <string>
#include <frontend/rules/rules.hpp>

class StateVector : public std::vector<Rules::Cases> {
public:
    StateVector() = default;

    StateVector(const std::vector<Rules::Cases> &o):
        std::vector<Rules::Cases>(o) {}
    explicit operator std::string() const;
    bool contains(const Rules::Cases &val) const;
    bool child_of(const StateVector &other) const {return other.size() < size();}
};


#endif //C_FORMAT_CHECKER_STATE_VECTOR_HPP
