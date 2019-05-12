#ifndef LEXER_H
#define LEXER_H

#include "frontend/lexer/coords.hpp"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <frontend/rules/rules.hpp>

namespace lexem {
enum Type {
    LPAREN,    // (
    RPAREN,    // )
    LBRACKET,  // [
    RBRACKET,  // ]
    LBRACE,    // {
    RBRACE,    // }
    LANGLE,    // <
    RANGLE,    // >
    BACKSLASH, // '\'
    SEMICOLON, // ;
    COLON,     // :
    COMMA,     // ,
    DOLLAR,    // $
    DOT,       // .
    ELLIPSIS,  // ...
    IDENT,     // [a-zA-Z][a-zA-Z0-9]*
    STRING,    // ".*"
    CHAR,      // '\n'
    NUM,       // [0-9a-fA-FuUlLxX]+(\.[0-9a-fA-FuUlLxX]+)?([eEpP][+-]?[0-9a-fA-FuUlLxX]+)?
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
    CASE,
    DEFAULT,
    DO,
    ELSE,
    ENUM,
    FOR,
    IF,
    STRUCT,
    SWITCH,
    WHILE,


//    WHITESPACE, // [' ''\t']+
    NEWLINE,    // [\r\n][WHITESPACE\r\n]*
    END_OF_FILE,
    ERROR   // now used only for uninitialized tokens
};

std::string to_string(lexem::Type t);

} //namespace lexem


class Token {
protected:
    lexem::Type m_type = lexem::Type::ERROR;
    std::string_view m_image;
    Coords m_start;
    Coords m_follow;
public:
    Rules::Cases rule = Rules::Cases::BLOCK;

    Token() = default;
    Token(lexem::Type type, std::string_view image, Coords start, Coords follow);
    Token(Token &&tok) noexcept;
    Token(const Token &tok) = default;
    Token& operator=(const Token&) = default;
    inline bool operator==(lexem::Type type) const { return m_type == type; }
    inline bool operator!=(lexem::Type type) const { return m_type != type; }
    lexem::Type       type()   const;
    std::string_view  image()  const;
    std::string       image_escaped() const;
    Coords            start()  const;
    Coords            follow() const;
    inline std::string coords_to_string() const { return Coords::coords_to_string(m_start, m_follow); }
    inline bool in(const std::vector<lexem::Type> &set) const {
        return std::find(set.begin(), set.end(), m_type) != set.end();
    }
    inline bool isEOF() const { return m_type == lexem::END_OF_FILE;}
    inline bool notEOF() const { return m_type != lexem::END_OF_FILE;}
//    bool in(std::initializer_list<lexem::Type> values);
    explicit operator std::string() const;
};

std::ostream& operator<<(std::ostream& os, const Token& obj);

#endif // LEXER_H
