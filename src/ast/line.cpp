#include "line.hpp"
#include <sstream>

#define GREEN_TEXT(x) (std::string("\033[1;32m") + x + std::string("\033[0m"))

Line::Line(Indent indent)
    : m_indent(indent)
{

}

Line::operator std::string() const
{
    std::stringstream ss;
    if (!empty()) {
        const Token &elem = front();
        ss << elem.start().get_line() <<":  ";
    }
    ss << m_indent.len() << " " << std::string(m_state);
    ss << " {";
    for (const Token &tok: *this) {
        ss << GREEN_TEXT(tok.image_escaped()) << ", ";
    }
    ss << "}";
    return ss.str();
}

void Line::reset_indent(Indent new_indent) {
    this->m_indent = new_indent;
}

void Line::addState(const std::vector<Rules::Cases> &rule_cases)
{
    m_state = rule_cases;
}

Rules::Cases Line::popState()
{
    Rules::Cases back = m_state.back();
    m_state.pop_back();
    return back;
}

Rules::Cases &Line::backState() {
    return m_state.back();
}