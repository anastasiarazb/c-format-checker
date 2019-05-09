#include "state.hpp"
#include <sstream>

#define GREEN_TEXT(x) (std::string("\033[1;32m") + x + std::string("\033[0m"))

State::State()
{

}

void State::set_rule(int level, Rules::Cases rule)
{
    this->level = level;
    this->rule = rule;
}

State::operator std::string() const
{
    std::stringstream ss;
    ss << "State: " << level << " " << to_string(rule) << " { ";
    for (const Token &tok: *this) {
        ss << GREEN_TEXT(tok.image_escaped()) << ", ";
    }
    ss << "}";
    return ss.str();
}
