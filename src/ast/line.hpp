#ifndef C_FORMAT_CHECKER_LINE_HPP
#define C_FORMAT_CHECKER_LINE_HPP

#include "frontend/rules/rules.hpp"
#include "indent.hpp"
#include "state_vector.hpp"
#include <string>


class Line : public std::vector<Token> {
    Indent m_indent;
    StateVector m_state;
public:
    explicit Line(Indent indent);
    void addState(const std::vector<Rules::Cases> &rule_cases);
    Rules::Cases popState();
    Rules::Cases &backState();
    Indent indent() const {return m_indent;}
    const StateVector &states() const {return m_state;}
    void reset_indent(Indent new_indent);
    explicit operator std::string() const;
};


#endif //C_FORMAT_CHECKER_LINE_HPP
