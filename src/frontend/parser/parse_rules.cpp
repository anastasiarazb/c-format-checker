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
//    Coords fragment_start = token.start();

    while (token != lexem::END_OF_FILE) {
        parse_statement(1);
    }

//    Coords fragment_end = token.follow();
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
          | labeled_statement
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
        case lexem::CASE:
        case lexem::DEFAULT:
            parse_labeled_statement(level + 1);
            break;
        case lexem::IDENT:
        {
            Token next = scanner.peekToken();
            if (next == lexem::COLON) {
                parse_labeled_statement(level + 1);
            } else {
                parse_simple_expr(level + 1);
            }
            break;
        }
        default:
            parse_simple_expr(level+1);
    }
    Coords fragment_end = token.start();
    LOG(0, GREEN_TEXT(get_image(fragment_start, fragment_end)));
    LOG(level, std::string(" ") + __func__ + std::string(", next = ") + std::string(token) << "\n\n");
}

/*
labeled_statement = IDENT ':'
                  | CASE word_sequence ':'
                  | DEFAULT ':'
 */

void Parser::parse_labeled_statement(int level)
{
    LOG(level, std::string(" ") + __func__ + std::string(", first = ") + std::string(token));
    Coords fragment_start = token.start();

    if (token == lexem::IDENT) {  // [... LABEL] IDENT ; [... LABEL STATEMENT] ':' ; [...] ...
        rule_cases.push_back(Rules::Cases::LABEL);
        lines.correctState(rule_cases);  // [... LABEL]
        rule_cases.push_back(Rules::Cases::STATEMENT);  // [... LABEL STATEMENT]
        nextToken();
        CHECK_TOKEN({lexem::COLON}, {lexem::COLON});  // ':'
        popCase(false);  // [... LABEL]
        popCase(false);  // [...]
        nextToken();  // follow
    } else if (token == lexem::CASE) {

    } else if (token == lexem::DEFAULT) {

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
        parse_nested_statement(level + 1);
    } else if (token == lexem::WHILE) {  // WHILE '(' word_sequence ')' statement
        pushCase(Rules::Cases::STATEMENT);
        nextToken();
        CHECK_TOKEN({lexem::LPAREN}, {lexem::LPAREN});
        nextToken();

        parse_word_sequence(level + 1);

        CHECK_TOKEN({ lexem::RPAREN }, { lexem::RPAREN });
        popCase();
        parse_nested_statement(level + 1);
    } else if (token == lexem::DO) {  // DO statement WHILE '(' word_sequence ')' ';'
        parse_nested_statement(level + 1);

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

        parse_nested_statement(level + 1);

        if (token == lexem::ELSE) {
            parse_nested_statement(level + 1);
        }
    } // else if (token == lexem::SWITCH)

    Coords fragment_end = token.start();
    LOG(0, GREEN_TEXT(get_image(fragment_start, fragment_end)));
    LOG(level, std::string(" ") + __func__ + std::string(", next = ") + std::string(token) << "\n\n");
}


/* Helper function for parse_selection_statement & parse_iteration_statement
 */
void Parser::parse_nested_statement(int level)
{
    bool one_line_stmt = (scanner.peekToken() != lexem::LBRACE);
    // allow_shift: allow make braces to the right of main statement (otherwise open-close baraces have
    // the same nest level as leading if-else-for-while-do-switch expression (more troubles than profits)
    bool allow_shift = false;
    std::cout << "Token & peek " << token << scanner.peekToken() << one_line_stmt << std::endl;
    if (one_line_stmt || allow_shift) {
        pushCase(Rules::Cases::IF_ELSE_WHILE_DO);
    }
    nextToken();

    parse_statement(level);

    if (one_line_stmt || allow_shift) {
        popCase();
    }
}

/*
simple_statement = word_sequence SEMICOLON
                 | word_sequence // if word_sequence scanned function_definition or label
*/
void Parser::parse_simple_expr(int level)
{
    LOG(level, std::string(" ") + __func__ + std::string(", first = ") + std::string(token));
    Coords fragment_start = token.start();

    pushCase(Rules::Cases::STATEMENT);
    Rules::Cases parsed_rule = parse_word_sequence(level + 1);
    if (parsed_rule == Rules::Cases::STATEMENT) {
        CHECK_TOKEN({ lexem::SEMICOLON }, { lexem::SEMICOLON, lexem::RBRACE });
        popCase();
        nextToken();
    } else if (parsed_rule == Rules::Cases::LABEL){
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
word = first \ {LBRACE, LPAREN, STRUCT, UNION, ENUM}
word_sequence = (word | '(' word_sequence ')' | initializer_list | union_struct_enum_definition)*
              | function_definition
function_definition = word_sequence IDENT '(' word_sequence ')' block
 */
Rules::Cases Parser::parse_word_sequence(int level)
{
    static const std::vector<lexem::Type> first = {
        lexem::LBRACKET,  // [
        lexem::RBRACKET,  // ]
        lexem::LANGLE,    // <
        lexem::RANGLE,    // >
        lexem::COLON,     // :
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
        lexem::QUESTIONMARK, // ?
        lexem::TYPENAME,
        lexem::TYPEDEF,
        lexem::KEYWORD,

        lexem::STRUCT,
        lexem::UNION,
        lexem::ENUM,

        lexem::DOUBLEHASH,
        lexem::LBRACE,
        lexem::LPAREN
    };
    LOG(level, std::string(" ") + __func__ + std::string(", first = ") + std::string(token));
    Coords fragment_start = token.start();
    auto exit_actions = [&]() {
        Coords fragment_end = token.start();
        LOG(0, GREEN_TEXT(get_image(fragment_start, fragment_end)));
        LOG(level, std::string(" ") + "parse_word_sequence" + std::string(", next = ") + std::string(token) << "\n\n");
    };
    bool func_suspicious = false;  // check for pattern IDENT '(' word_sequence ')' '{'
//    int len = 0;
    while (token.in(first)) {
//        ++len;
        switch(token.type()) {
            case lexem::LBRACE:
                if (func_suspicious) { // function definition found => parse block
                    popCase();  // close statement and start block
                    parse_block(level+1);
                    exit_actions();
                    return Rules::Cases::FUNCTION;
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
            case lexem::STRUCT:
            case lexem::UNION:
            case lexem::ENUM:
                parse_union_struct_enum_definition(level + 1);
                continue;
//            case lexem::COLON:
//                if (last_token == lexem::IDENT && len == 2) { // IDENT ':'
//                    pushCase(Rules::Cases::LABEL);
//                    popCase();
//                    exit_actions();
//                    return Rules::Cases::LABEL;
//                } else {  // copy of default case
//                    nextToken();
//                    func_suspicious = false;
//                    continue;
//                }
            default: {
                nextToken();
                func_suspicious = false;
            }
        }
    }
    exit_actions();
    return Rules::Cases::STATEMENT;
}


/*
initializer_list = '{' word_sequence '}'
*/
void Parser::parse_initializer_list(int level, Rules::Cases rule_to_push)
{
    LOG(level, std::string(" ") + __func__ + std::string(", first = ") + std::string(token));
    Coords fragment_start = token.start();

    CHECK_TOKEN({lexem::LBRACE}, {lexem::LBRACE});

    pushCase(rule_to_push);
    nextToken();
    parse_word_sequence(level + 1);
    popCase();

    CHECK_TOKEN({lexem::RBRACE}, {lexem::RBRACE});
    nextToken();

    Coords fragment_end = token.start();
    LOG(0, GREEN_TEXT(get_image(fragment_start, fragment_end)));
    LOG(level, std::string(" ") + __func__ + std::string(", next = ") + std::string(token) << "\n\n");
}


/*
 union_struct_enum_definition = (STRUCT | UNION) IDENT? block  # declaration
                             | (STRUCT | UNION) IDENT  # usage
                             | ENUM IDENT? initializer_list  # declaration
                             | ENUM IDENT  # usage
*/
void Parser::parse_union_struct_enum_definition(int level)
{
    LOG(level, std::string(" ") + __func__ + std::string(", first = ") + std::string(token));
    Coords fragment_start = token.start();
    Rules::Cases rule_case;
    switch (token.type()) {
        case lexem::STRUCT:
            rule_case = Rules::Cases::STRUCT;
            break;
        case lexem::UNION:
            rule_case = Rules::Cases::UNION;
            break;
        case lexem::ENUM:
            rule_case = Rules::Cases::ENUM;
            break;
        default:
            throw std::logic_error("Expected token of types {STRUCT, UNION, ENUM}, got " + std::string(token) \
                + " at " + __FUNCTION__ + ", " + std::to_string(__LINE__));
    }

    nextToken();
    CHECK_TOKEN({lexem::IDENT, lexem::LBRACE}, {lexem::IDENT, lexem::LBRACE});
    if (token == lexem::IDENT) {
        nextToken();
    }
    // allow_shift: allow make braces to the right of main statement (otherwise open-close baraces have
    // the same nest level as leading if-else-for-while-do-switch expression
    bool allow_shift = false;
    if (allow_shift) {
        pushCase(rule_case);
    }
    if (token == lexem::LBRACE) {
        if (rule_case == Rules::Cases::STRUCT || rule_case == Rules::Cases::UNION) {
            parse_block(level + 1);
        } else {  // rule_case == Rules::Cases::ENUM
            // for enums rules are stricter than for ordinary initializer lists: set block structure
            parse_initializer_list(level + 1, Rules::Cases::BLOCK);
            nextToken();
        }
    }
    if (allow_shift) {
        popCase();
    }

    Coords fragment_end = token.start();
    LOG(0, GREEN_TEXT(get_image(fragment_start, fragment_end)));
    LOG(level, std::string(" ") + __func__ + std::string(", next = ") + std::string(token) << "\n\n");
}