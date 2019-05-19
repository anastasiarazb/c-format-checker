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
    analyze();
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
                + " at " + __FUNCTION__ + ", " + std::to_string(__LINE__) + ": " + std::string(line));
            }
        }
    }

    std::cout << std::string(*this) << std::endl;
}

void Analyzer::collect_stats()
{
//    std::vector<Line>::iterator it = begin();
    for (Line &line: *this) {
        stats[line.states()].push_back(line.indent());
    }
}

std::string Analyzer::str_stats() const
{
    std::stringstream ss;
    for (const std::pair<const StateVector, std::vector<Indent>> &elem: stats) {
        for (const Indent &id: elem.second) {
            ss << std::string(elem.first) << " " << std::string(id) << "\n";
        }
    }
    return ss.str();
}

void Analyzer::analyze()
{
    bool whitespaces = false;
    bool tabs = false;
    for (const auto &[state, indents]: stats) {
        Indent standard = indents.front();
        for (const Indent &ind: indents) {
            whitespaces += ind.spaces();
            tabs += ind.tabs();
            if (ind.len() == standard.len()) {
                if (ind.mixed()) {
                    error_list.push_back(wrap_error(state, ind, "предупреждение", "Использование пробелов и табуляций."));
                    std::cout << error_list.back() << std::endl;
                } else if (ind.tabs() != 0) {
                    error_list.push_back(wrap_error(state, ind, "предупреждение",
                                                    "Использование табуляций (ранее использовались пробелы)."));
                    std::cout << error_list.back() << std::endl;
                }
            } else if (rules[state.back()].count(Rules::Indent::ANY) == 0){
                error_list.push_back(wrap_error(state, ind, standard, "ошибка"));
                std::cout << error_list.back() << std::endl;
            }
        }
    }
}

std::string Analyzer::wrap_error(const StateVector &state, const Indent &err_ind, const Indent &standard
                                  , const std::string &level, const std::string &assumption) const
{
    std::stringstream ss;
    ss << "[Анализ отступов], строка "<< err_ind.follow().get_line() << ": " <<  level
       << ": отступ ширины " << err_ind.len() << ": <" << err_ind.image_escaped()
       << ">. Ранее на том же уровне вложенности в строке " << standard.follow().get_line()
       << " отступ ширины " << standard.len()
       << ": <" << standard.image_escaped() << ">. "
       << assumption;
    return ss.str();
}

std::string Analyzer::wrap_error(const StateVector &state, const Indent &err_ind
                                  , const std::string &level, const std::string &assumption) const
{
    std::stringstream ss;
    ss << "[Анализ отступов], строка "<< err_ind.follow().get_line() << ": " <<  level
       << ": отступ ширины " << err_ind.len() << ": <" << err_ind.image_escaped() << ">. "
       << assumption;
    return ss.str();
}
