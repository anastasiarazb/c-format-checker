#ifndef C_FORMAT_CHECKER_INDENT_HPP
#define C_FORMAT_CHECKER_INDENT_HPP

#include <frontend/lexer/token.hpp>

class Indent {
    int tabs = -1;
    int spaces = -1;
public:
    Indent() = default;
//    Indent(const Indent &) = default;
//    Ident(Indent &&) = default;
    explicit Indent(Token newline_tok);
    void update(Token newline_tok);
    bool mixed() const { return tabs > 0 && spaces > 0; }
    int  len(int tablen=4) const { return spaces + tabs * tablen; }
    bool initialized() const {return tabs < 0 || spaces < 0;}
};


#endif //C_FORMAT_CHECKER_INDENT_HPP
