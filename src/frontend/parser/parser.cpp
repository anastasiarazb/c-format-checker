#include <sstream>
#include "parser.hpp"


Parser::Parser(Scanner &scanner) :
    scanner(scanner)
{
    lines.emplace_back(Indent());
}

Parser::State Parser::saveState()
{
    state = {cur_indent, token, lines, rule_cases, scanner.saveState(), errors_list};
    return state;
}

void Parser::restoreState()
{
    cur_indent = state.cur_indent;
    token      = state.token;
    lines      = state.lines;
    rule_cases = state.rule_cases;
    scanner.restoreState(state.scanner_state);
    errors_list = state.errors_list;
}

Parser::State Parser::restoreState(const Parser::State &backup)
{
    saveState();
    cur_indent = backup.cur_indent;
    token = backup.token;
    lines = backup.lines;
    rule_cases = backup.rule_cases;
    scanner.restoreState(backup.scanner_state);
    errors_list = backup.errors_list;
    return state;
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

const Token& Parser::nextToken()
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
        lines.back().addState(rule_cases);
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
