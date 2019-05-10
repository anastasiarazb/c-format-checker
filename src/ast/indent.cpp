#include "indent.hpp"

Indent::Indent(Token newline_tok)
{
    update(newline_tok);
}

void Indent::update(Token newline_tok)
{
    tabs = spaces = 0;
    for (auto it = newline_tok.image().end()-1; *it != '\n' && *it != '\r'; --it)
    {
        if (*it == ' ') {
            ++spaces;
        } else if (*it == '\t') {
            ++tabs;
        } else {
            throw std::logic_error("got unexpected symbol <" + std::string(1, *it) \
                                   + "> at " + __FILE__ + \
                                   ", line " + std::to_string(__LINE__));
        }
    }
}