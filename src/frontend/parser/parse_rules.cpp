#include <ast/analyzer.hpp>
#include "parser.hpp"

#define CHECK_TOKEN(...) checkToken(__VA_ARGS__, __FILE__, __LINE__)
#define LOG(level, ...) \
{ \
    for (int i = 0; i < level; ++i) std::cout <<  '>';  \
    std::cout << __VA_ARGS__ << std::endl; \
}
#define GREEN_TEXT(x) (x.empty() ? std::string("@@") : std::string("@\033[1;32m") + std::string(x) + "\033[0m@")


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
          | iteration_statement
          | selection_statement
*/
void Parser::parse_statement(int level)
{
    LOG(level, std::string(" ") + __func__ + std::string(", first = ") + std::string(token));
    Coords fragment_start = token.start();
    switch (token.type()) {
        case lexem::LBRACE:
            parse_block(level+1);
            break;
        case lexem::FOR:
        case lexem::WHILE:
        case lexem::DO:
            parse_iteration_statement(level+1);
            break;
        case lexem::IF:
        case lexem::SWITCH:
            parse_selection_statement(level+1);
            break;
        default:
            parse_simple_expr(level+1);
    }
    Coords fragment_end = token.start();
    LOG(0, GREEN_TEXT(get_image(fragment_start, fragment_end)));
    LOG(level, std::string(" ") + __func__ + std::string(", next = ") + std::string(token) << "\n\n");
}


/*
iteration_statement = FOR '(' word_sequence ';' word_sequence ';' word_sequence ')' statement
                    | WHILE '(' word_sequence ')' statement
                    | DO statement WHILE '(' word_sequence ')' ';'
 */
void Parser::parse_iteration_statement(int level)
{
    LOG(level, std::string(" ") + __func__ + std::string(", first = ") + std::string(token));
    Coords fragment_start = token.start();

    if (token == lexem::FOR) {  // FOR '(' word_sequence ';' word_sequence ';' word_sequence ')' statement
        pushCase(Rules::Cases::STATEMENT);
        nextToken();
        CHECK_TOKEN({ lexem::LPAREN }, { lexem::LPAREN });
        nextToken();

        parse_word_sequence(level + 1);
        CHECK_TOKEN({ lexem::SEMICOLON }, { lexem::SEMICOLON });
        nextToken();
        parse_word_sequence(level + 1);
        CHECK_TOKEN({ lexem::SEMICOLON }, { lexem::SEMICOLON });
        nextToken();
        parse_word_sequence(level + 1);

        CHECK_TOKEN({ lexem::RPAREN }, { lexem::RPAREN });
        popCase();
        nextToken();

        parse_statement(level + 1);
    } else if (token == lexem::WHILE) {  // WHILE '(' word_sequence ')' statement
        pushCase(Rules::Cases::STATEMENT);
        nextToken();
        CHECK_TOKEN({lexem::LPAREN}, {lexem::LPAREN});
        nextToken();

        parse_word_sequence(level + 1);

        CHECK_TOKEN({ lexem::RPAREN }, { lexem::RPAREN });
        popCase();
        nextToken();

        parse_statement(level + 1);
    } else if (token == lexem::DO) {  // DO statement WHILE '(' word_sequence ')' ';'
        nextToken();

        parse_statement(level + 1);

        CHECK_TOKEN({ lexem::WHILE }, { lexem::WHILE });
        pushCase(Rules::Cases::STATEMENT);
        nextToken();
        CHECK_TOKEN({lexem::LPAREN}, {lexem::LPAREN});
        nextToken();

        parse_word_sequence(level + 1);

        CHECK_TOKEN({ lexem::RPAREN }, { lexem::RPAREN });
        nextToken();
        CHECK_TOKEN({ lexem::SEMICOLON }, { lexem::SEMICOLON });
        popCase();
        nextToken();

    }


    Coords fragment_end = token.start();
    LOG(0, GREEN_TEXT(get_image(fragment_start, fragment_end)));
    LOG(level, std::string(" ") + __func__ + std::string(", next = ") + std::string(token) << "\n\n");
}


/*
selection_statement = IF '(' word_sequence ')' statement (ELSE statement) ?
                    | SWITCH '(' word_sequence ')' statement
*/
void Parser::parse_selection_statement(int level)
{
    LOG(level, std::string(" ") + __func__ + std::string(", first = ") + std::string(token));
    Coords fragment_start = token.start();

    if (token == lexem::IF) {  // IF '(' word_sequence ')' statement (ELSE statement) ?
        pushCase(Rules::Cases::STATEMENT);
        nextToken();
        CHECK_TOKEN({lexem::LPAREN}, {lexem::LPAREN});
        nextToken();

        parse_word_sequence(level + 1);

        CHECK_TOKEN({ lexem::RPAREN }, { lexem::RPAREN });
        popCase();

        Scanner::State scan_state = scanner.saveState();
        bool one_line_stmt = (scanner.nextToken() != lexem::RBRACE);
        scanner.restoreState(scan_state);
        if (one_line_stmt) {
            pushCase(Rules::Cases::IF_ELSE_WHILE_DO);
        }
        nextToken();

        parse_statement(level + 1);

        if (one_line_stmt) {
            popCase();
        }

        if (token == lexem::ELSE) {
            nextToken();
            parse_statement(level + 1);
        }
    } // else if (token == lexem::SWITCH)

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
    if (last_case != Rules::Cases::BLOCK) {
        CHECK_TOKEN({ lexem::SEMICOLON }, { lexem::SEMICOLON, lexem::RBRACE });
        popCase();
        nextToken();
    } else {
        popCase();
    }
//    std::cout << token << " "  << std::endl;


    Coords fragment_end = token.start();
    LOG(0, GREEN_TEXT(get_image(fragment_start, fragment_end)));
    LOG(level, std::string(" ") + __func__ + std::string(", next = ") + std::string(token) << "\n\n");
}

/*
block = '{' statement* '}'
 */
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


/*
word = first \ {LBRACE, LPAREN}
word_sequence = (word | '(' word_sequence ')' | initializer_list)* (IDENT '(' word_sequence ')' block) ?
 */
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
    LOG(level, std::string(" ") + __func__ + std::string(", first = ") + std::string(token));
    Coords fragment_start = token.start();
    bool func_suspicious = false;  // check for pattern IDENT '(' word_sequence ')' '{'
    bool parsed_function = false;
    while (token.in(first) && !parsed_function) {
        switch(token.type()) {
        case lexem::LBRACE:
            if (func_suspicious) { // function definition found => parse block
                parse_block(level+1);
                parsed_function = true;
                break;
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
    Coords fragment_end = token.start();
    LOG(0, GREEN_TEXT(get_image(fragment_start, fragment_end)));
    LOG(level, std::string(" ") + __func__ + std::string(", next = ") + std::string(token) << "\n\n");
}

/*
initializer_list = '{' word_sequence '}'
*/

void Parser::parse_initializer_list(int level)
{
    LOG(level, std::string(" ") + __func__ + std::string(", first = ") + std::string(token));
    Coords fragment_start = token.start();

    CHECK_TOKEN({lexem::LBRACE}, {lexem::LBRACE});
    nextToken();

    parse_word_sequence(level + 1);

    CHECK_TOKEN({lexem::RBRACE}, {lexem::RBRACE});
    nextToken();

    Coords fragment_end = token.start();
    LOG(0, GREEN_TEXT(get_image(fragment_start, fragment_end)));
    LOG(level, std::string(" ") + __func__ + std::string(", next = ") + std::string(token) << "\n\n");
}
