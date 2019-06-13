#include "analyzer.hpp"
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <algorithm>


#define LOG(...) \
{ \
    /*logs <<  __VA_ARGS__ << std::endl;*/ \
    if (params.log_level == LogLevel::DEBUG) std::cout << __VA_ARGS__ << std::endl; \
}


Analyzer::Analyzer(const TokenTable &other, Rules rules,  const Params &params) :
    TokenTable(other), rules(std::move(rules)), params(params)
{
    preprocess_pass();
    collect_stats();
    LOG(str_stats());
//    analyze();
//    std::cout << error_messages() << std::endl;
}

Analyzer::operator std::string() const
{
    std::stringstream ss;
    for (const Line &line: *this) {
        ss << std::string(line) << "\n";
    }
    return ss.str();
}

void Analyzer::preprocess_pass()
{
    // trunc last line if it is empty
    auto last_line = --end();
    if (last_line->front() == lexem::END_OF_FILE) {
        erase(last_line);
    }
}


// Check that if one statement is nested then it is not to left to the parent
void Analyzer::check_nested()
{
    auto prev_line = cbegin();
    for(auto line = cbegin(); line != cend(); ++line) {
        const StateVector state = line->states();
        const StateVector state_prev = prev_line->states();
        if (state.child_of(state_prev)
            && line->indent() < prev_line->indent())
        {
            if (!state.contains(Rules::Cases::LABEL) && !(state.back() == Rules::Cases::STATEMENT)) {
                std::string message = "Вложенное выражение левее объемлющего.";
                add_error("ошибка", message, line->indent());
            }
        }
        if (state.empty() || state.back() != Rules::Cases::STATEMENT) {
            prev_line = line;
        }
    }
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
    check_nested();

    bool whitespaces = false;
    bool tabs = false;
    for (const auto &[state, indents]: stats) {
        Indent standard = indents.front();
        for (const Indent &ind: indents) {
            whitespaces += ind.spaces();
            tabs += ind.tabs();

            // Case of statement splitted by lines: check to be not to the left of the previous line
            if (!state.empty() && state.back() == Rules::Cases::STATEMENT) {
                int line = ind.start().get_line();  // number of the line in the program
                int index = vectorIndex(line);      // index of the line in token table
                const Line &parent = at(index-1);   // error cannot occur in the first line => [index-1] is ok
                Indent parent_indent = parent.indent();
                if (ind.len() < parent_indent.len()) {
                    const char * message = params.log_level != LogLevel::QUIET
                                           ? "Продолжение выражения левее предыдущей строки."
                                           : "Ошибка отступа при переносе выражения.";
                    add_error("ошибка", message, ind);
                }
            }

            if (ind.mixed()) {
                const char * message = params.log_level != LogLevel::QUIET
                        ? "Использование пробелов и табуляций в одном отступе."
                        : "Использование табуляции.";
                add_error("ошибка", message, ind);
                exit_code = EXIT_CODE_MIXED_SPACES;
                return;
//                    std::cout << error_list.back() << std::endl;
            } else if (ind.tabs() != 0) {
                const char * message = params.log_level != LogLevel::QUIET
                        ? "Использование табуляций (ранее использовались пробелы)."
                        : "Использование табуляции.";
                add_error("ошибка", message, ind);
                exit_code = EXIT_CODE_MIXED_SPACES;
                return;
//                    std::cout << error_list.back() << std::endl;
            }

            if (ind.len() == standard.len()) {
                standard = ind;  // make standard the last one with the same properties

            } else {
                if (state.empty() || rules[state.back()].count(Rules::Indent::ANY) == 0) {
                    const char * message = params.log_level == LogLevel::QUIET
                            ? "Различные отступы на одном уровне вложенности."  // In QUIET mode only this message would be shown
                            : "";  // There would be full output provided by wrap_error
                    add_error("ошибка", message, ind, true, standard);
                }
//                std::cout << error_list.back() << std::endl;
            }


        }
    }
}

std::string Analyzer::wrap_error(const std::string &error_level, const std::string &assumption,
                                 const Indent &wrong_indent, bool add_standard, const Indent &standard_indent) const
{
    std::stringstream ss;

    ss << params.message_header(wrong_indent.line()) << " ";
    if (params.log_level == LogLevel::QUIET) {
        ss << assumption;
        return ss.str();
    }
    ss << error_level << ": отступ ширины " << wrong_indent.len() << ": <" << wrong_indent.image_escaped()  << ">. ";
    if (add_standard) {
        ss << "Ранее на том же уровне вложенности в строке " << standard_indent.follow().get_line()
           << " отступ ширины " << standard_indent.len()
           << ": <" << standard_indent.image_escaped() << ">. ";
    }
    ss  << assumption;
    return ss.str();
}

void Analyzer::add_error(const std::string &error_level, const std::string &assumption,
                         const Indent &wrong_indent, bool add_standard, const Indent &standard_indent)
{
    if (exit_code == EXIT_CODE_OK) {
        exit_code = EXIT_CODE_INDENT_ERROR;
    }
    std::string message = wrap_error(error_level, assumption, wrong_indent, add_standard, standard_indent);
//    std::cout << message << std::endl;
    error_list[wrong_indent.line()] = message;
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


//void Analyzer::pushToken(Token token)
//{
//    back().push_back(token);
//}
//
//void Analyzer::addLine(Indent indent)
//{
//    emplace_back(indent);
//}
//
//Token &Analyzer::lastToken()
//{
//    return back().back();
//}