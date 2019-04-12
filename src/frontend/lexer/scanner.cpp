#include "frontend/lexer/scanner.hpp"
#include "common.h"
#include <cctype>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream

/*
class Scanner {
    std::string program;
    std::unordered_map<std::string_view, lexem::Type> keywords2type;
    const char *start_ptr = nullptr;
    const char *cur_ptr = nullptr;
    Coords start_pos;
    Coords cur_pos;
    char  cur_char;
    int   token_len;
    void  skipWhitespaces();
    enum CommentStyle {
        ONELINE, MULTYLINE
    };
    void  skipComment(CommentStyle type);
    Token scanIdent();
    bool  isKeyword(std::string_view word);
    bool  reachedEOF();
    Token scanNum();
    Token scanString();
    Token scanChar();
    void  nextChar();
    inline char peekNext();
    void  clearCounters();
    void  rollbackCounters();
    Token tokenOfType(lexem::Type type);
    void  add_error(std::string message);
    std::list<std::string> errors_list;
public:
    Scanner(const char *path);
    Token nextToken();
    void  print_errors();
};

*/

std::ostream& operator<<(std::ostream& os, const Token& obj)
{
    return os << to_string(obj.type()) <<
              ": <" << obj.image() << ">";
}

Scanner::Scanner(const char *path)
{
    std::ifstream file (path, std::ios::in);
    if (file) {
        file.seekg(0, std::ios::end);
        program.reserve(file.tellg());
        file.seekg(0, std::ios::beg);
        program.assign((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
        start_ptr = program.c_str();
        cur_ptr = start_ptr;
        cur_char = *(cur_ptr);
        token_len = 0;
    } else {
        printf("No such file %s\n", path);
        exit(2);
    }

    keywords2type["auto"] = lexem::AUTO;
    keywords2type["case"] = lexem::CASE;
    keywords2type["const"] = lexem::CONST;
    keywords2type["default"] = lexem::DEFAULT;
    keywords2type["do"] = lexem::DO;
    keywords2type["else"] = lexem::ELSE;
    keywords2type["enum"] = lexem::ENUM;
    keywords2type["for"] = lexem::FOR;
    keywords2type["while"] = lexem::WHILE;
    keywords2type["goto"] = lexem::GOTO;
    keywords2type["if"] = lexem::IF;
    keywords2type["label"] = lexem::LABEL;
    keywords2type["return"] = lexem::RETURN;
    keywords2type["struct"] = lexem::STRUCT;
    keywords2type["switch"] = lexem::SWITCH;
    keywords2type["then"] = lexem::THEN;
    keywords2type["typedef"] = lexem::TYPEDEF;
    keywords2type["while"] = lexem::WHILE;
}

inline std::string_view Scanner::image() const
{
    return std::string_view(start_ptr, token_len);
}

inline Token Scanner::tokenOfType(lexem::Type type)
{
#ifdef DEBUG_LEXER
    std::cout << "  TOKEN " << to_string(type) << " " << Coords::coords_to_string(start_pos, cur_pos) <<
              " <" << image() << ">" << std::endl;
#endif
    return Token(type, image(), start_pos, cur_pos);
}

void Scanner::skipWhitespaces() {
    while(isspace(*cur_ptr)) {
        cur_pos.shift(*cur_ptr);
        ++cur_ptr;
    }
}

inline char Scanner::peekNext() {
    return *(cur_ptr + 1);
}

void Scanner::skipComment(CommentStyle type) {
    nextChar();
    auto generate_error = [this](const char *expected_sequence) {
        std::string message = std::string("Error: comment at ") + Coords::coords_to_string(start_pos, cur_pos)
                              + std::string(" is open, ") + std::string(expected_sequence)
                              + std::string(" expected, end of file reached");
        errors_list.push_back(message);
    };
    switch (type) {
        case CommentStyle::MULTYLINE:
            while (!reachedEOF()
                   && ! (cur_char == '*' && peekNext() == '/')) {
                nextChar();
            }
            if (reachedEOF()) {
                generate_error("'*/'");
                return;
            }
            nextChar(); // '/'
            nextChar(); // next
            return;
        case CommentStyle::ONELINE:
            while (!reachedEOF() && cur_char != '\n') {
                nextChar();
            }
            if (reachedEOF()) {
                generate_error("<newline>");
                return;
            }
            nextChar();
            return;
    }
}



Token Scanner::nextToken()
{
    skipWhitespaces();
    clearCounters();
    switch (cur_char) {
        case '(':
            nextChar();
            return tokenOfType(lexem::LPAREN);
        case ')':
            nextChar();
            return tokenOfType(lexem::RPAREN);
        case '[':
            nextChar();
            return tokenOfType(lexem::LBRACKET);
        case ']':
            nextChar();
            return tokenOfType(lexem::RBRACKET);
        case '{':
            nextChar();
            return tokenOfType(lexem::LBRACE);
        case '}':
            nextChar();
            return tokenOfType(lexem::RBRACE);
        case '\\':
            nextChar();
            return tokenOfType(lexem::BACKSLASH);
        case ';':
            nextChar();
            return tokenOfType(lexem::SEMICOLON);
        case '+':
            nextChar();
            if (cur_char == '+') {
                nextChar();
                return tokenOfType(lexem::INC_DEC);   // '++'
            } else if (cur_char == '=') {
                nextChar();
                return tokenOfType(lexem::ASSIGNOP);  // '+='
            }
            return tokenOfType(lexem::OPERATOR); // '+'
        case '-':
            nextChar();
            if (cur_char == '-') {
                nextChar();
                return tokenOfType(lexem::INC_DEC);  // '--'
            } else if (cur_char == '>') {
                nextChar();
                return tokenOfType(lexem::ARROW);  // '->'
            } else if (cur_char == '=') {
                nextChar();
                return tokenOfType(lexem::ASSIGNOP);  // '-='
            }
            return tokenOfType(lexem::OPERATOR);  // '-'
        case '/':
            nextChar();
            if (cur_char == '/') {
                skipComment(CommentStyle::ONELINE);  // '//'
                return nextToken();
            } else if (cur_char == '=') {
                nextChar();
                return tokenOfType(lexem::ASSIGNOP);  // '/='
            }
            return tokenOfType(lexem::OPERATOR);  // '/'

        case '~':
            nextChar();
            return tokenOfType(lexem::OPERATOR);  // '~'
        case '%':
        case '^':
            nextChar();
            if (cur_char == '=') {
                nextChar();
                return tokenOfType(lexem::ASSIGNOP);  // '%=', '^='
            }
            return tokenOfType(lexem::OPERATOR);  // '%', '^'
        case '|':
            nextChar();
            if (cur_char == '|') {
                nextChar();
                return tokenOfType(lexem::OPERATOR);  // '||'
            } else if (cur_char == '=') {
                nextChar();
                return tokenOfType(lexem::ASSIGNOP);  // '|='
            }
            return tokenOfType(lexem::OPERATOR);  // '%', '|', '^'
        case '$':
            nextChar();
            return tokenOfType(lexem::DOLLAR);
        case '*':
            nextChar();
            if (cur_char == '=') {
                nextChar();
                return tokenOfType(lexem::ASSIGNOP);  // '*='
            }
            return tokenOfType(lexem::STAR);  // '*' : a * b, *p
        case '&':
            nextChar();
            if (cur_char == '&') {
                nextChar();
                return tokenOfType(lexem::OPERATOR); // '&&'
            } else if (cur_char == '=') {
                nextChar();
                return tokenOfType(lexem::ASSIGNOP);  // '&='
            }
            return tokenOfType(lexem::AMPERSAND);  // '&' : a & b, &p
        case '<':
            nextChar();
            if (cur_char == '=') { // '<='
                nextChar();
            } else if (cur_char == '<') {
                nextChar();
                if (cur_char == '=') {
                    nextChar();
                    return tokenOfType(lexem::ASSIGNOP);  // '<<='
                }
                return tokenOfType(lexem::OPERATOR);  // '<<'
            }
            return tokenOfType(lexem::COMPAREOP); // '<', '<='
        case '>':
            nextChar();
            if (cur_char == '=') {  // '>='
                nextChar();
            } else if (cur_char == '>') {
                nextChar();
                if (cur_char == '=') {
                    nextChar();
                    return tokenOfType(lexem::ASSIGNOP);  // '>>='
                }
                return tokenOfType(lexem::OPERATOR);  // '>>'
            }
            return tokenOfType(lexem::COMPAREOP);  // '>', '>='
        case '!':
            nextChar();
            if (cur_char == '=') {  // '!='
                nextChar();
                return tokenOfType(lexem::COMPAREOP);
            }
            return tokenOfType(lexem::OPERATOR); // '!'
        case ':':
            nextChar();
            return tokenOfType(lexem::COLON);
        case '\'':
            return scanChar();
        case '"':
            return scanString();
        case '=':
            nextChar();
            if (cur_char == '=') { // ==
                nextChar();
                return tokenOfType(lexem::COMPAREOP);
            } else {  // =
                return tokenOfType(lexem::ASSIGNOP);
            }
        case '#':
            nextChar();
            if (cur_char == '#') {
                nextChar();
                return tokenOfType(lexem::DOUBLEHASH);
            }
            return tokenOfType(lexem::HASH);

        case '?':
            nextChar();
            return tokenOfType(lexem::QUESTIONMARK);
        case ',':
            nextChar();
            return tokenOfType(lexem::COMMA);
        case '.':
            nextChar();
            if (cur_char == '.') {
                nextChar();
                if (cur_char == '.') {
                    nextChar();
                    return tokenOfType(lexem::ELLIPSIS);
                } else {
                    add_error("Unexpected '.' after '.'");
                }
            }
            return tokenOfType(lexem::DOT);
        case '\0':
            return tokenOfType(lexem::END_OF_FILE);
        default:
            if (isalpha(cur_char) || cur_char == '_') {
                return scanIdent();
            } else if (isdigit(cur_char)) {
                return scanNum();
            } else {
                nextChar();
                std::string error_message =
                        std::string("Error: Undeхpected symbol '") + std::string(start_ptr, cur_ptr) + std::string("' at ")
                        + Coords::coords_to_string(start_pos, cur_pos);
                errors_list.push_back(error_message);
                return nextToken();

            }
    }
}

//Прочитать следующий символ из программы
void Scanner::nextChar()
{
    cur_pos.shift(cur_char);
    ++cur_ptr;
    cur_char = *cur_ptr;
    ++token_len;
}

//Стартовая инициализация при начале считывания нового токена
inline void Scanner::clearCounters()
{
    start_ptr = cur_ptr;
    cur_char  = *cur_ptr;
    start_pos = cur_pos;
    token_len = 0;
}

void Scanner::add_error(std::string message)
{
    std::string str = "Error at " + cur_pos.to_string() + ": "
                      + message;
    errors_list.push_back(str);
}

//Реинициализация при неудавшейся попытке считывания последнего токена
inline void Scanner::rollbackCounters()
{
    cur_ptr   = start_ptr;
    cur_char  = *cur_ptr;
    cur_pos   = start_pos;
    token_len = 0;
}

bool Scanner::reachedEOF()
{
    return cur_char == '\0';
}

Token Scanner::scanIdent() {
    while (isalpha(cur_char) || isdigit(cur_char) || cur_char == '_') {
        nextChar();
    }
    auto search = keywords2type.find(image());
    if (search != keywords2type.end()) {
        return tokenOfType((*search).second);
    }
    return tokenOfType(lexem::IDENT);
}

Token Scanner::scanString() {
    nextChar();
    while (cur_char != '"') {
        if (reachedEOF()) {
            add_error("Unexpected END_OF_FILE during string scanning");
        }
        if (cur_char == '\\' && peekNext() == '\"') { // Пропустить экранированную кавычку ''
            nextChar();
        }
        nextChar();
    }
    nextChar();
    return tokenOfType(lexem::STRING);
}

Token Scanner::scanChar() {
    nextChar();
    if (cur_char == '\\') { // e.g. '\n'
        nextChar();
    }
    nextChar(); // char
    if (cur_char != '\'') {
        std::string error_message =
                std::string("Improper char declaration, uneхpected symbol '")
                + cur_char + "', '\'' expected.";
        add_error(error_message);
    }
    nextChar();
    return tokenOfType(lexem::CHAR);
}

Token Scanner::scanNum() {
    auto skip_digit_sequence = [this]() {
        while (isdigit(cur_char)) {
            nextChar();
        }
    };
    auto skip_digit_char_sequence = [this]() {
        while (isdigit(cur_char) || isalpha(cur_char)) {
            nextChar();
        }
    };
    skip_digit_sequence();
    if (cur_char == '.') { // fractional part
        nextChar();
        skip_digit_sequence();
    }
    if (cur_char == 'e' || cur_char == 'E' || cur_char == 'x' || cur_char == 'X') { // scale factor | hexadecimal
        nextChar();
        if (cur_char == '-' || cur_char == '+') {
            nextChar();
        }
        skip_digit_char_sequence();
    }
    return tokenOfType(lexem::NUM);
}

void Scanner::print_errors() {
    for (const auto & message: errors_list) {
        std::cout << message << std::endl;
    }
}
