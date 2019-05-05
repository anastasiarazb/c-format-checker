#include "parser.hpp"

#define CHECK_TOKEN(a, b) checkToken(a, b, __FILE__, __LINE__)
#define LOG(level, ...) \
{ \
    for (int i = 0; i < level; ++i) std::cout <<  '>';  \
    std::cout << __VA_ARGS__ << std::endl; \
}
#define GREEN_TEXT(x) (std::string("@\033[1;32m") + std::string(x) + std::string("\033[0m@"))


void Parser::parse()
{
    LOG(0, "Parse top level");
    nextToken();
    Coords fragment_start = token.start();
    while (token == lexem::NEWLINE) {
        nextToken();
    }
    while (token != lexem::END_OF_FILE) {
        parse_simple_expr(1);
    }
    Coords fragment_end = token.follow();
//    LOG(get_image(fragment_start, fragment_end));
}

void Parser::parse_pragma(int level)
{
    LOG(level, std::string(" ") + __func__ + std::string(", first = ") + std::string(token));
    Coords fragment_start = token.start();
    while (token != lexem::Type::NEWLINE && token != lexem::END_OF_FILE) {
        nextToken(RETURN_NEWLINES);
        if (token == lexem::Type::BACKSLASH) {
            nextToken(RETURN_NEWLINES);
            CHECK_TOKEN({lexem::Type::NEWLINE}, {lexem::Type::NEWLINE});
            nextToken(RETURN_NEWLINES); // skip newline to allow multi-line defines
        }
    }
    Coords fragment_end = token.follow();
    while (token == lexem::NEWLINE) {
        nextToken();
    }
    LOG(0, GREEN_TEXT(get_image(fragment_start, fragment_end)));
    LOG(level, std::string(" ") + __func__ + std::string(", next = ") + std::string(token) << "\n\n");

}


/*
simple_statement = word_sequence (OPERATOR block)? SEMICOLON
                 | block
*/
void Parser::parse_simple_expr(int level)
{
    LOG(level, std::string(" ") + __func__ + std::string(", first = ") + std::string(token));
    Coords fragment_start = token.start();
    std::vector<lexem::Type> first = {
        lexem::LBRACKET,  // [
        lexem::RBRACKET,  // ]
        lexem::LANGLE,    // <
        lexem::RANGLE,    // >
        lexem::COMMA,     // ,
        lexem::DOLLAR,    // $
        lexem::DOT,       // .
        lexem::ELLIPSIS,  // ...
        lexem::IDENT,     // [a-zA-Z][a-zA-Z0-9]*
        lexem::STRING,    // ".*"
        lexem::CHAR,      // '\n'
        lexem::NUM,       // [0-9a-fA-FuUlLxX]+(\.
        lexem::INC_DEC,   // ++, --
        lexem::OPERATOR,  // +, -, /, %, ~, |, ^, <<, >>, !, &&, ||
        lexem::ARROW,     // ->
        lexem::STAR,      // *
        lexem::ASSIGNOP,  // =, +=, -=, *=, /=, %=, &=, |=, ^=, <<=, >>=
        lexem::COMPAREOP, // >, <, !=, <=, =>, ==
        lexem::AMPERSAND, // &

        lexem::DOUBLEHASH,
        lexem::HASH,
        lexem::LBRACE
    };
    if (token == lexem::SEMICOLON) {  // empty statement
        nextToken();
        Coords fragment_end = token.start();
        LOG(0, GREEN_TEXT(get_image(fragment_start, fragment_end)));
        LOG(level, std::string(" ") + __func__ + std::string(", next = ") + std::string(token) << "\n\n");
        return;
    }
    CHECK_TOKEN(first, {lexem::SEMICOLON});
    if (token == lexem::SEMICOLON || token.isEOF()) {  // broken statement
        nextToken();
        Coords fragment_end = token.start();
        LOG(0, GREEN_TEXT(get_image(fragment_start, fragment_end)));
        LOG(level, std::string(" ") + __func__ + std::string(", next = ") + std::string(token) << "\n\n");
        return;
    }
    while (token.in(first))
    {
        switch(token.type()){
        case lexem::HASH:
            parse_pragma(level+1);
            continue;
        case lexem::LBRACE:
            parse_block(level+1);
            continue;
        default:
            nextToken();
        }
    }
    if (token == lexem::SEMICOLON) {
        nextToken();
    }
    Coords fragment_end = token.start();
    LOG(0, GREEN_TEXT(get_image(fragment_start, fragment_end)));
    LOG(level, std::string(" ") + __func__ + std::string(", next = ") + std::string(token) << "\n\n");
}

void Parser::parse_block(int level)
{
//    if (level > 5) exit(1);
    LOG(level, std::string(" ") + __func__ + std::string(", first = ") + std::string(token));
    Coords fragment_start = token.start();

    CHECK_TOKEN({lexem::LBRACE}, {lexem::LBRACE});
    nextToken();
    while (token.notEOF() && token != lexem::RBRACE) {
        parse_simple_expr(level + 1);
    }
    CHECK_TOKEN({lexem::RBRACE}, {lexem::RBRACE});
    nextToken();

    Coords fragment_end = token.start();
    LOG(0, GREEN_TEXT(get_image(fragment_start, fragment_end)));
    LOG(level, std::string(" ") + __func__ + std::string(", next = ") + std::string(token) << "\n\n");
}


/// TODO: разделить сущности "блок" и "лист инициализации",
/// блок = simple_statement, иниц_лист = word ВНУТРИ стейтмента
