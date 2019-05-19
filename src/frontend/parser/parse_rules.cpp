#include "parser.hpp"

#define CHECK_TOKEN(...) checkToken(__VA_ARGS__, __FILE__, __LINE__)
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

    while (token != lexem::END_OF_FILE) {
        parse_statement(1);
    }

    Coords fragment_end = token.follow();
//    LOG(get_image(fragment_start, fragment_end));
}

void Parser::parse_pragma(int level)
{
    LOG(level, std::string(" ") + __func__ + std::string(", first = ") + std::string(token));
    Coords fragment_start = token.start();
    while (token != lexem::Type::NEWLINE && token != lexem::END_OF_FILE) {
        nextTokenPragma();
        if (token == lexem::Type::BACKSLASH) {
            nextTokenPragma();
            CHECK_TOKEN({lexem::Type::NEWLINE}, {lexem::Type::NEWLINE});
            nextTokenPragma(); // skip newline to allow multi-line defines
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
statement = block
           | simple_expr
*/
void Parser::parse_statement(int level)
{
    LOG(level, std::string(" ") + __func__ + std::string(", first = ") + std::string(token));
    Coords fragment_start = token.start();
    switch (token.type()) {
        case lexem::LBRACE:
            parse_block(level+1);
            break;
        default:
            parse_simple_expr(level+1);
    }
    Coords fragment_end = token.start();
    LOG(0, GREEN_TEXT(get_image(fragment_start, fragment_end)));
    LOG(level, std::string(" ") + __func__ + std::string(", next = ") + std::string(token) << "\n\n");
}


/*
simple_statement = word_sequence SEMICOLON
                 | word_sequence // if the last popped case was BLOCK, what means function definition
*/
void Parser::parse_simple_expr(int level)
{
    LOG(level, std::string(" ") + __func__ + std::string(", first = ") + std::string(token));
    Coords fragment_start = token.start();

    pushCase(Rules::Cases::STATEMENT);
    parse_word_sequence(level + 1);
    if (last_case != Rules::Cases::BLOCK) CHECK_TOKEN({lexem::SEMICOLON}, {lexem::SEMICOLON, lexem::RBRACE});
    popCase();
    nextToken();

    Coords fragment_end = token.start();
    LOG(0, GREEN_TEXT(get_image(fragment_start, fragment_end)));
    LOG(level, std::string(" ") + __func__ + std::string(", next = ") + std::string(token) << "\n\n");
}


void Parser::parse_block(int level)
{
//    if (level > 5) exit(1);
    LOG(level, std::string(" ") + __func__ + std::string(", first = ") + std::string(token));
    Coords fragment_start = token.start();
    std::cout << "LEVEL = " << level << std::string(fragment_start) << std::endl;

    CHECK_TOKEN({lexem::LBRACE}, {lexem::LBRACE});
    pushCase(Rules::Cases::BLOCK);
    nextToken();
    while (token.notEOF() && token != lexem::RBRACE) {
        parse_statement(level + 1);
    }
    popCase();
    CHECK_TOKEN({lexem::RBRACE}, {lexem::RBRACE});
    nextToken();

    Coords fragment_end = token.start();
    LOG(0, GREEN_TEXT(get_image(fragment_start, fragment_end)));
    LOG(level, std::string(" ") + __func__ + std::string(", next = ") + std::string(token) << "\n\n");
}

void Parser::parse_word_sequence(int level)
{
    static const std::vector<lexem::Type> first = {
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
        lexem::TYPENAME,
        lexem::TYPEDEF,
        lexem::KEYWORD,

        lexem::DOUBLEHASH,
        lexem::LBRACE,
        lexem::LPAREN
    };
    bool func_suspicious = false;  // check for pattern IDENT '(' word_sequence ')' '{'
    while (token.in(first)) {
        switch(token.type()) {
        case lexem::LBRACE:
            if (func_suspicious) { // function definition found => parse block
                parse_block(level+1);
            } else {
                parse_initializer_list(level+1);
            }
            func_suspicious = false;
            continue;
        case lexem::LPAREN:
            func_suspicious = (last_token == lexem::IDENT);
            nextToken();
            parse_word_sequence(level+1);
            CHECK_TOKEN({lexem::RPAREN}, {lexem::RPAREN});
            nextToken();
            continue;
        default:
            nextToken();
            func_suspicious = false;
        }
    }

}

void Parser::parse_initializer_list(int level)
{
    LOG(level, std::string(" ") + __func__ + std::string(", first = ") + std::string(token));
    Coords fragment_start = token.start();

    CHECK_TOKEN({lexem::LBRACE}, {lexem::LBRACE});
    nextToken();
    while (token.notEOF() && token != lexem::RBRACE) {
        parse_word_sequence(level + 1);
    }
    CHECK_TOKEN({lexem::RBRACE}, {lexem::RBRACE});
    nextToken();

    Coords fragment_end = token.start();
    LOG(0, GREEN_TEXT(get_image(fragment_start, fragment_end)));
    LOG(level, std::string(" ") + __func__ + std::string(", next = ") + std::string(token) << "\n\n");
}
