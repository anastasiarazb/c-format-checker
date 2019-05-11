#include "line.hpp"
#include <sstream>

#define GREEN_TEXT(x) (std::string("\033[1;32m") + x + std::string("\033[0m"))

Line::Line(Indent indent)
    : indent(indent)
{

}

Line::operator std::string() const
{
    std::stringstream ss;
    ss << indent.len() << " " << "[ ";
    for (Rules::Cases c: state) {
        ss << to_string(c) << " ";
    }
    ss << "] {";
    for (const Token &tok: *this) {
        ss << GREEN_TEXT(tok.image_escaped()) << ", ";
    }
    ss << "}";
    return ss.str();
}

void Line::reset_indent(Indent new_indent) {
    this->indent = new_indent;
}

void Line::addState(const std::vector<Rules::Cases> &rule_cases)
{
    state = rule_cases;
}

Rules::Cases Line::popState()
{
    Rules::Cases back = state.back();
    state.pop_back();
    return back;
}

Rules::Cases &Line::backState() {
    return state.back();
}