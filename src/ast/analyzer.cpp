#include "analyzer.hpp"
#include <sstream>
#include <iostream>

Analyzer::Analyzer(const std::vector<Line> &other, Rules rules) :
    std::vector<Line>(other), rules(std::move(rules))
{
    first_pass();
}

Analyzer::operator std::string() const
{
    std::stringstream ss;
    for (const Line &line: *this) {
        ss << std::string(line) << "\n";
    }
    return ss.str();
}

void Analyzer::first_pass()
{
    for(Line &line: *this) {
        Token &first_sym = line.front();
        if (first_sym == lexem::Type::RBRACE) {
            Rules::Cases state = line.popState();
            if (state != Rules::Cases::BLOCK) {
                throw std::logic_error("Expected Rules::Cases::BLOCK, got " + to_string(state) \
                + " at " + __FUNCTION__ + ", " + std::to_string(__LINE__));
            }
        }
    }

    std::cout << std::string(*this) << std::endl;
}

void Analyzer::collect_stats()
{
    for (Line &line: *this) {

    }
}