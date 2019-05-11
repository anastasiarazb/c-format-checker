#ifndef C_FORMAT_CHECKER_LINE_HPP
#define C_FORMAT_CHECKER_LINE_HPP

#include "frontend/rules/rules.hpp"
#include "state.hpp"
#include "indent.hpp"
#include <string>


class Line : public std::vector<Token> {
    Indent indent;
    std::vector<Rules::Cases> state;
public:
    explicit Line(Indent indent);
    void addState(const std::vector<Rules::Cases> &rule_cases);
    Rules::Cases popState();
    void reset_indent(Indent new_indent);
    explicit operator std::string() const;
};


#endif //C_FORMAT_CHECKER_LINE_HPP
