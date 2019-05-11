#include <sstream>
#include "parser.hpp"


Parser::Parser(Scanner &scanner) :
    scanner(scanner)
{
    lines.emplace_back(Indent());
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


const Token& Parser::nextTokenPragma()
{

    token = scanner.nextToken();
    if (token == lexem::Type::NEWLINE) {
        cur_indent.update(token);
        if (lines.back().empty()) {
            lines.pop_back();
        }
        lines.emplace_back(cur_indent);
    }
//    std::cout << token << std::endl;
    return token;
}

const Token& Parser::nextToken(SkipNewlines skip_newlines)
{

    token = scanner.nextToken();
    if (token == lexem::Type::NEWLINE) {
        while (token == lexem::Type::NEWLINE) {
            cur_indent.update(token);
            token = scanner.nextToken();
        }
        if (lines.back().empty()) {
            lines.pop_back();
        }
        lines.emplace_back(cur_indent);
    }
    switch (token.type()) {
        case lexem::Type::BACKSLASH:
            return nextToken();
        case lexem::Type::HASH:
            parse_pragma();
            return token;
        default:
            lines.back().push_back(token);
            return token;
    }
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
