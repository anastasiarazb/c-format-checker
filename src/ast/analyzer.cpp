#include "analyzer.hpp"
#include <sstream>
#include <iostream>
#include <unordered_map>

Analyzer::Analyzer(const std::vector<Line> &other, Rules rules) :
    std::vector<Line>(other), rules(std::move(rules))
{
    first_pass();
    collect_stats();
    std::cout << str_stats() << std::endl;
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
//    std::vector<Line>::iterator it = begin();
    for (Line &line: *this) {
        stats.emplace(line.states(), line.indent());
        std::cout << line.front().image() << std::endl;
    }
}

std::string Analyzer::str_stats() const
{
    std::stringstream ss;
    for (std::pair<StateVector, Indent> elem: stats) {
        ss << std::string(elem.first)
           << " " << std::string(elem.second) << "\n";
    }
    return ss.str();
}