#include "parser.hpp"

#define PROCESS_ERROR(a, b) process_error(a, b, __FILE__, __LINE__)
#define LOG(level, ...) \
{ \
    for (int i = 0; i < level; ++i) std::cout <<  '>';  \
    std::cout << __VA_ARGS__ << std::endl; \
}
#define GREEN_TEXT(x) (std::string("@\033[1;32m") + std::string(x) + std::string("\033[0m@"))


void Parser::parse_pragma(int level)
{
    LOG(level, std::string("Parse pragma, first = ") + std::string(token));
    Coords fragment_start = token.start();
    while (token != lexem::Type::NEWLINE && token != lexem::END_OF_FILE) {
        nextToken(RETURN_NEWLINES);
        if (token == lexem::Type::BACKSLASH) {
            nextToken(RETURN_NEWLINES);
            if (token != lexem::Type::NEWLINE) {
                PROCESS_ERROR(lexem::Type::NEWLINE, lexem::Type::NEWLINE);
            }
            nextToken(RETURN_NEWLINES); // skip newline to allow multi-line defines
        }
    }
    Coords fragment_end = token.follow();
    while (token == lexem::NEWLINE) {
        nextToken();
    }
    LOG(0, GREEN_TEXT(get_image(fragment_start, fragment_end)));
    LOG(level, std::string("Parse pragma, last = ") + std::string(token) << "\n\n");

}

void Parser::parse()
{
    LOG(0, "Parse top level");
    nextToken();
    Coords fragment_start = token.start();
    while (token == lexem::NEWLINE) {
        nextToken();
    }
    while (token != lexem::END_OF_FILE) {
        if (token == lexem::HASH) {
            parse_pragma(1);
        }
    }
    Coords fragment_end = token.follow();
//    LOG(get_image(fragment_start, fragment_end));
}