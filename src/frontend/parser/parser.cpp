#include <sstream>
#include "parser.hpp"

Indent::Indent(Token newline_tok)
{
    update(newline_tok);
}

void Indent::update(Token newline_tok)
{
    tabs = spaces = 0;
    for (auto it = newline_tok.image().end()-1; *it != '\n' && *it != '\r'; --it) {
        if (*it == ' ') {
            ++spaces;
        } else if (*it == '\t') {
            ++tabs;
        } else {
            throw std::logic_error("got unexpected symbol <" + std::string(1, *it) + "> at " + __FILE__ + \
                                   ", line " + std::to_string(__LINE__));
        }
    }
}

Parser::Parser(Scanner &scanner) :
    scanner(scanner)
{

}

void Parser::readNewlines()
{
    while (token == lexem::NEWLINE) {
        cur_indent.update(token);
        nextToken();
    }
}

//const Token& Parser::nextToken()
//{
//
//    token = scanner.nextToken();
//    if (token == lexem::NEWLINE) {
//        cur_indent.update(token);
//    }
//    std::cout << token << std::endl;
//    return token;
//}

const Token& Parser::nextToken(SkipNewlines skip_newlines)
{

    token = scanner.nextToken();
    if (token == lexem::NEWLINE) {
        if (skip_newlines == RETURN_NEWLINES) {
            cur_indent.update(token);
            goto NEXT_TOKEN_RETURN;
        } else {
            while (token == lexem::NEWLINE) {
                cur_indent.update(token);
                token = scanner.nextToken();
            }
            goto NEXT_TOKEN_RETURN;
        }
    }
    NEXT_TOKEN_RETURN:
//    std::cout << token << std::endl;
    return token;
}

std::string_view Parser::get_image(Coords start, Coords follow) const
{
    return scanner.substring(start, follow);
}