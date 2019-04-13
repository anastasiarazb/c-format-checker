#include "frontend/lexer/token.hpp"
#include "common.h"
#include <algorithm>


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
        RETURN_NAME(BACKSLASH);
        RETURN_NAME(SEMICOLON);
        RETURN_NAME(COLON);
        RETURN_NAME(COMMA);
        RETURN_NAME(SINGLE_QUOTE);
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
        RETURN_NAME(AUTO);
        RETURN_NAME(CASE);
        RETURN_NAME(CONST);
        RETURN_NAME(DEFAULT);
        RETURN_NAME(DO);
        RETURN_NAME(ELSE);
        RETURN_NAME(ENUM);
        RETURN_NAME(FOR);
        RETURN_NAME(GOTO);
        RETURN_NAME(IF);
        RETURN_NAME(LABEL);
        RETURN_NAME(RETURN);
        RETURN_NAME(STRUCT);
        RETURN_NAME(SWITCH);
        RETURN_NAME(THEN);
        RETURN_NAME(TYPEDEF);
        RETURN_NAME(WHILE);
        RETURN_NAME(WHITESPACE);
        RETURN_NAME(END_OF_FILE);
        RETURN_NAME(ERROR);
        default: return "undefined lex type";
    }
#undef RETURN_NAME
}

Token::Token(lexem::Type type, std::string_view image, Coords start, Coords follow):
    m_type(type), m_image(image), m_start(start), m_follow(follow) {}

Token::Token(Token &&tok):
    m_type(tok.m_type), m_image(tok.m_image), m_start(tok.m_start), m_follow(tok.m_follow) {}

Token::Token(const Token &tok):
    m_type(tok.m_type), m_image(tok.m_image), m_start(tok.m_start), m_follow(tok.m_follow) {}

lexem::Type Token::type() const {
    return m_type;
}

std::string_view Token::image() const {
    return m_image;
}

Coords Token::start() const {
    return m_start;
}

Coords Token::follow() const {
    return m_follow;
}

Token::operator std::string() const
{
    std::string repr = lexem::to_string(m_type) + " " + coords_to_string() + ": <" + std::string(m_image) + ">";
    return repr;
}

std::ostream& operator<<(std::ostream& os, const Token& obj)
{
    return os << std::string(obj);
}
