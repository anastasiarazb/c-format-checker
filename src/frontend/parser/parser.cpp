#include <sstream>
#include "parser.hpp"


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
            lines.emplace_back(cur_indent);
            goto NEXT_TOKEN_RETURN;
        } else {
            while (token == lexem::NEWLINE) {
                cur_indent.update(token);
                token = scanner.nextToken();
            }
            lines.emplace_back(cur_indent);
            goto NEXT_TOKEN_RETURN;
        }
    }
    NEXT_TOKEN_RETURN:
    lines.back().push_back(token);
//    std::cout << token << std::endl;
    return token;
}

std::string_view Parser::get_image(Coords start, Coords follow) const
{
    return scanner.substring(start, follow);
}

std::string Parser::get_lines() const
{
    std::stringstream ss;
    for (const Line &line: lines) {
        ss << std::string(line) << "\n";
    }
    return ss.str();
}
