#ifndef C_FORMAT_CHECKER_INDENT_HPP
#define C_FORMAT_CHECKER_INDENT_HPP

#include <frontend/lexer/token.hpp>

class Indent: public Token {
    int m_tabs = 0;
    int m_spaces = 0;
public:
    Indent() = default;
//    Indent(const Indent &) = default;
//    Ident(Indent &&) = default;
    explicit Indent(const Token &newline_tok);
    explicit operator std::string() const;
    bool operator==(const Indent &other) const;
    bool operator<(const Indent &other) const {return len() < other.len();}
    bool operator>(const Indent &other) const {return len() > other.len();}
    void update(Token newline_tok);
    bool mixed() const { return m_tabs > 0 && m_spaces > 0; }
    int  len(int tablen=4) const { return m_spaces + m_tabs * tablen; }
    int  line() const { return follow().get_line(); }
    int  tabs() const { return m_tabs; }
    int  spaces() const { return m_spaces; }
};


#endif //C_FORMAT_CHECKER_INDENT_HPP
