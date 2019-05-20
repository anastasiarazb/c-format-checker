#ifndef C_FORMAT_CHECKER_PARSER_HPP
#define C_FORMAT_CHECKER_PARSER_HPP

#include "ast/indent.hpp"
#include "ast/line.hpp"
#include <frontend/lexer/scanner.hpp>
#include <frontend/rules/rules.hpp>

class Parser {
    struct State {
        Indent cur_indent;
        Token token;
        Token last_token;
        std::vector<Line> lines;
        std::vector<Rules::Cases> rule_cases;
        Rules::Cases last_case;

        Scanner::State scanner_state;
        std::list<std::string> errors_list;
    } state;
    Indent cur_indent;
    Token token;
    Token last_token;
    std::vector<Line> lines;
    std::vector<Rules::Cases> rule_cases;
    Rules::Cases last_case;

    Scanner &scanner;
    std::list<std::string> errors_list;

    void         pushCase(Rules::Cases rule_case) {rule_cases.push_back(rule_case);}
    Rules::Cases popCase() {last_case = rule_cases.back(); rule_cases.pop_back(); return last_case;}
    const Token &nextTokenPragma();
    const Token &nextToken();
    void         write_message(const std::string &message, char const *file, int line);
    const Token &process_error(lexem::Type expected, lexem::Type skip_until);
    const Token &process_error(const std::vector<lexem::Type> &expected,
                               const std::vector<lexem::Type> &skip_until,
                               char const *file, int line);
    const Token &checkToken(const std::vector<lexem::Type> &expected,
                            const std::vector<lexem::Type> &skip_until,
                            char const *file, int line);
public:
    explicit Parser(Scanner &scan);
    State saveState();
    void restoreState();
    State restoreState(const State &backup);
    std::string get_errors_list() const;
    std::string get_lines() const;
    void readNewlines();
    std::string_view get_image(Coords start, Coords follow) const;
    std::vector<Line> &get_token_table() {return lines;};

    void parse();
    void parse_pragma(int level = 0);
    void parse_statement(int level = 0);
    void parse_iteration_statement(int level = 0);
    void parse_selection_statement(int level = 0);
    void parse_simple_expr(int level = 0);
    void parse_block(int level = 0);
    void parse_word_sequence(int level = 0);
    void parse_initializer_list(int level = 0);
};


#endif //C_FORMAT_CHECKER_PARSER_HPP
