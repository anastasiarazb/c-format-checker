#include "indent.hpp"
#include "sstream"

Indent::Indent(Token newline_tok) : Token(newline_tok)
{
    update(newline_tok);
}

void Indent::update(Token newline_tok)
{
    Token::m_image = newline_tok.image();
    Token::m_start = newline_tok.start();
    Token::m_follow = newline_tok.follow();
    Token::m_type = newline_tok.type();

    m_tabs = m_spaces = 0;
    std::string_view image = newline_tok.image();
    for (auto it = image.end()-1; *it != '\n' && *it != '\r'; --it)
    {
        if (*it == ' ') {
            ++m_spaces;
        } else if (*it == '\t') {
            ++m_tabs;
        } else {
            throw std::logic_error("got unexpected symbol <" + std::string(1, *it) \
                                   + "> at " + __FILE__ + \
                                   ", line " + std::to_string(__LINE__));
        }
    }

    Token::m_image.remove_prefix(std::min(Token::m_image.find_first_not_of("\r\n"),
                                          Token::m_image.size()));
    for (char c: image) {
        if (c == ' ' || c == '\t') break;
        Token::m_start.shift(c);
    }
}

Indent::operator std::string() const
{
    std::stringstream ss;
    ss << len() << " " << std::string(static_cast<Token>(*this));
    return ss.str();
}

