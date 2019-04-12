#ifndef LEXER_H
#define LEXER_H

#include "frontend/lexer/coords.hpp"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace lexem {
enum Type {
    LPAREN,    // (
    RPAREN,    // )
    LBRACKET,  // [,
    RBRACKET,  // ],
    LBRACE,    // {
    RBRACE,    // }
    BACKSLASH, // '\'
    SEMICOLON, // ;
    COLON,     // :
    COMMA,     // ,
    SINGLE_QUOTE, // '
    DOLLAR,    // $
    DOT,       // .
    ELLIPSIS,  // ...
    IDENT,     // [a-zA-Z][a-zA-Z0-9]*
    STRING,    // ".*"
    CHAR,      // '\n'
    NUM,       // [0-9]*
    INC_DEC,   // ++, --
    OPERATOR,  // +, -, /, %, ~, |, ^, <<, >>, !, &&, ||
    ARROW,     // ->
    STAR,      // *
    ASSIGNOP,  // =, +=, -=, *=, /=, %=, &=, |=, ^=, <<=, >>=
    COMPAREOP, // >, <, !=, <=, =>, ==
    HASH,      // #
    DOUBLEHASH,// ##
    AMPERSAND, // &
    QUESTIONMARK, // ?
    /* KEYWORDS */
    AUTO,
    CASE,
    CONST,
    DEFAULT,
    DO,
    ELSE,
    ENUM,
    FOR,
    GOTO,
    IF,
    LABEL,
    RETURN,
    STRUCT,
    SWITCH,
    THEN,
    TYPEDEF,
    WHILE,

    END_OF_FILE,
    ERROR
};
} //namespace lexem

std::string to_string(lexem::Type t);

class Token {
    lexem::Type m_type;
    std::string_view m_image;
    Coords m_start;
    Coords m_follow;
public:
    Token() = default;
    Token(lexem::Type type, std::string_view image, Coords start, Coords follow);
    Token(Token &&tok);
    Token(const Token &tok);
    Token& operator=(const Token&) = default;
    inline bool operator==(lexem::Type type) const { return m_type == type; }
    inline bool operator!=(lexem::Type type) const { return m_type != type; }
    lexem::Type       type()   const;
    std::string_view  image()  const;
    Coords            start()  const;
    Coords            follow() const;
    inline std::string coords_to_string() const { return Coords::coords_to_string(m_start, m_follow); }
    inline bool is_in(const std::vector<lexem::Type> &set) const {
        return std::find(set.begin(), set.end(), m_type) != set.end();
    }
};

std::ostream& operator<<(std::ostream& os, const Token& obj);

#endif // LEXER_H
