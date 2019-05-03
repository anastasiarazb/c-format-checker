#ifndef C_FORMAT_CHECKER_PARSER_HPP
#define C_FORMAT_CHECKER_PARSER_HPP


#include <frontend/lexer/scanner.hpp>
#include <frontend/rules/rules.hpp>

class Indent {
    int tabs = -1;
    int spaces = -1;
public:
    Indent() = default;
    explicit Indent(Token newline_tok);
    void update(Token newline_tok);
    bool mixed() const { return tabs > 0 && spaces > 0; }
    int  len(int tablen=4) const { return spaces + tabs * tablen; }
    bool initialized() const {return tabs < 0 || spaces < 0;}
};

class Parser {
    enum SkipNewlines {
        SKIP_NEWLINES, RETURN_NEWLINES
    };
    Indent cur_indent;
    Token token;
    std::vector<int> last_line_words_positions;
    std::vector<Rules::Cases> states;


//    const Token &nextToken();
    const Token &nextToken(SkipNewlines skip_newlines=SKIP_NEWLINES);
    Scanner &scanner;
    std::list<std::string> errors_list;
    const Token &process_error(lexem::Type expected, lexem::Type skip_until);
    const Token &process_error(lexem::Type expected,
                               const std::vector<lexem::Type> &skip_until);
    const Token &process_error(lexem::Type expected, lexem::Type skip_until,
                               char const *file, int line);
    const Token &process_error(lexem::Type expected,
                               const std::vector<lexem::Type> &skip_until,
                               char const *file, int line);
    const Token &process_error(const std::vector<lexem::Type> &expected,
                               const std::vector<lexem::Type> &skip_until,
                               char const *file, int line);
public:
    explicit Parser(Scanner &scan);
    std::string get_errors_list();
    void readNewlines();
    std::string_view get_image(Coords start, Coords follow) const;

    void parse();
    void parse_pragma(int level = 0);
    void parse_simple_expr(int level = 0);
};


#endif //C_FORMAT_CHECKER_PARSER_HPP
