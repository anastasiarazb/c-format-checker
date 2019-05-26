#ifndef C_FORMAT_CHECKER_PARSER_HPP
#define C_FORMAT_CHECKER_PARSER_HPP

#include "ast/indent.hpp"
#include "ast/line.hpp"
#include "ast/token_table.hpp"
#include <frontend/lexer/scanner.hpp>
#include <frontend/rules/rules.hpp>

class Parser {
    struct State {
        Indent cur_indent;
        Token token;
        Token last_token;
        TokenTable lines;
        std::vector<Rules::Cases> rule_cases;
        Rules::Cases last_case;

        Scanner::State scanner_state;
        std::list<std::string> errors_list;
    } state;
    Indent cur_indent;
    Token token;
    Token last_token;
    TokenTable lines;
    StateVector rule_cases;
    Rules::Cases last_case;

    Scanner &scanner;
    std::list<std::string> errors_list;

    void         pushCase(Rules::Cases rule_case) {rule_cases.push_back(rule_case);}
    Rules::Cases popCase(bool correct_current_token=true);
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
    TokenTable &get_token_table() {return lines;};

    void parse();
    void parse_pragma(int level = 0);
    void parse_statement(int level = 0);
    void parse_labeled_statement(int level = 0);
    void parse_iteration_statement(int level = 0);
    void parse_selection_statement(int level = 0);
    void parse_nested_statement(int level = 0);
    void parse_simple_expr(int level = 0);
    void parse_block(int level = 0);
    Rules::Cases parse_word_sequence(int level = 0);
    void parse_initializer_list(int level = 0, Rules::Cases rule_to_push=Rules::Cases::STATEMENT);
    void parse_union_struct_enum_definition(int level = 0);
};


#endif //C_FORMAT_CHECKER_PARSER_HPP
