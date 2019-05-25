#include "analyzer.hpp"
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <algorithm>

Analyzer::Analyzer(const std::vector<Line> &other, Rules rules) :
    std::vector<Line>(other), rules(std::move(rules))
{
//    first_pass();
    collect_stats();
    std::cout << str_stats() << std::endl;
    analyze();
    std::cout << error_messages() << std::endl;
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
            Rules::Cases state;
            do {
                state = line.popState();
            } while (state != Rules::Cases::BLOCK && state != Rules::Cases::STATEMENT);  // TODO: check correctness, try return only one pop with BLOCK check
//            Rules::Cases state = line.popState();
//            if (state != Rules::Cases::BLOCK) {
//                throw std::logic_error("Expected Rules::Cases::BLOCK, got " + to_string(state) \
                + " at " + __FUNCTION__ + ", " + std::to_string(__LINE__) + ": " + std::string(line));
//            }
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
    ss << "line: [state] indent type coords: image\n";
    for (const std::pair<const StateVector, std::vector<Indent>> &elem: stats) {
        for (const Indent &id: elem.second) {
            int coord_line = id.start().get_line();
            ss << coord_line << ":  " << std::string(elem.first) << " " << std::string(id) << "\n";
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
                    add_error(state, ind, "предупреждение", "Использование пробелов и табуляций.");
//                    std::cout << error_list.back() << std::endl;
                } else if (ind.tabs() != 0) {
                    add_error(state, ind, "предупреждение",
                                                    "Использование табуляций (ранее использовались пробелы).");
//                    std::cout << error_list.back() << std::endl;
                }
            } else if (rules[state.back()].count(Rules::Indent::ANY) == 0){
                add_error(state, ind, standard, "ошибка");
//                std::cout << error_list.back() << std::endl;
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

void Analyzer::add_error(const StateVector &state, const Indent &err_ind, const Indent &standard
    , const std::string &level, const std::string &assumption)
{
    std::string message = wrap_error(state, err_ind, standard, level, assumption);
//    std::cout << message << std::endl;
    error_list[err_ind.follow().get_line()] = message;
}
void Analyzer::add_error(const StateVector &state, const Indent &err_ind
    , const std::string &level, const std::string &assumption)
{
    std::string message = wrap_error(state, err_ind, level, assumption);
//    std::cout << message << std::endl;
    error_list[err_ind.follow().get_line()] = message;
}

std::string Analyzer::error_messages() const
{
//    std::sort(error_list.begin(), error_list.end());
    std::stringstream ss;
    for(const auto &[line, str]: error_list) {
        ss << str << std::endl;
    }
    return ss.str();
}


void Analyzer::pushToken(Token token)
{
    back().push_back(token);
}

void Analyzer::newline(Indent indent)
{
    emplace_back(indent);
}

Token &Analyzer::lastToken()
{
    return back().back();
}