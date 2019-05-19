#include "frontend/lexer/token.hpp"
#include "common.h"
#include <algorithm>
#include <unordered_map>


std::string lexem::to_string(lexem::Type t)
{
#define RETURN_NAME(p) case(lexem::p): return std::string(#p);
    switch(t){
        RETURN_NAME(LPAREN);
        RETURN_NAME(RPAREN);
        RETURN_NAME(LBRACKET);
        RETURN_NAME(RBRACKET);
        RETURN_NAME(LBRACE);
        RETURN_NAME(RBRACE);
        RETURN_NAME(LANGLE);
        RETURN_NAME(RANGLE);
        RETURN_NAME(BACKSLASH);
        RETURN_NAME(SEMICOLON);
        RETURN_NAME(COLON);
        RETURN_NAME(COMMA);
        RETURN_NAME(DOLLAR);
        RETURN_NAME(DOT);
        RETURN_NAME(ELLIPSIS);
        RETURN_NAME(IDENT);
        RETURN_NAME(STRING);
        RETURN_NAME(CHAR);
        RETURN_NAME(NUM);
        RETURN_NAME(INC_DEC);
        RETURN_NAME(OPERATOR);
        RETURN_NAME(ARROW);
        RETURN_NAME(STAR);
        RETURN_NAME(ASSIGNOP);
        RETURN_NAME(COMPAREOP);
        RETURN_NAME(HASH);
        RETURN_NAME(DOUBLEHASH);
        RETURN_NAME(AMPERSAND);
        RETURN_NAME(QUESTIONMARK);
        RETURN_NAME(CASE);
        RETURN_NAME(DEFAULT);
        RETURN_NAME(DO);
        RETURN_NAME(ELSE);
        RETURN_NAME(ENUM);
        RETURN_NAME(FOR);
        RETURN_NAME(IF);
        RETURN_NAME(STRUCT);
        RETURN_NAME(SWITCH);
        RETURN_NAME(WHILE);
        RETURN_NAME(TYPEDEF);
        RETURN_NAME(TYPENAME);
        RETURN_NAME(KEYWORD);
        RETURN_NAME(NEWLINE);
        RETURN_NAME(END_OF_FILE);
        RETURN_NAME(ERROR);
        default: return "undefined lex type";
    }
#undef RETURN_NAME
}

Token::Token(lexem::Type type, std::string_view image, Coords start, Coords follow):
    m_type(type), m_image(image), m_start(start), m_follow(follow) {}

Token::Token(Token &&tok) noexcept :
    m_type(tok.m_type), m_image(tok.m_image), m_start(tok.m_start), m_follow(tok.m_follow) {}

lexem::Type Token::type() const {
    return m_type;
}

std::string_view Token::image() const {
    return m_image;
}

std::string Token::image_escaped() const {
    if (m_type == lexem::END_OF_FILE) {
        return "<EOF>";
    }
    std::string escaped;
    escaped.reserve(m_image.size() * 2);
    static std::unordered_map<char, std::string> images ({
        {'\t', std::string("\\t")},
        {'\r', std::string("\\r")},
        {'\n', std::string("\\n")},
        {' ', std::string("␣")}
    });
    for (char c: m_image) {
        if (images.count(c)) {
            escaped.append(images[c]);
        } else {
            escaped.push_back(c);
        }
    }

    return escaped;
}

Coords Token::start() const {
    return m_start;
}

Coords Token::follow() const {
    return m_follow;
}

Token::operator std::string() const
{
    std::string res = lexem::to_string(m_type) + " " + coords_to_string() + ": <" + image_escaped() + ">";
    return res;
}

std::ostream& operator<<(std::ostream& os, const Token& obj)
{
    return os << std::string(obj);
}
