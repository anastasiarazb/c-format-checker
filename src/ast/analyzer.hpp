#ifndef C_FORMAT_CHECKER_ANALYZER_HPP
#define C_FORMAT_CHECKER_ANALYZER_HPP

#include "line.hpp"
#include "state_vector.hpp"
#include "token_table.hpp"
#include "frontend/lexer/token.hpp"
#include <vector>
#include <list>
#include <string>
#include <map>

class Analyzer: public TokenTable {
    Rules rules;
    std::map<StateVector, std::vector<Indent>> stats;
    std::map<int, std::string> error_list;
    std::string wrap_error(const StateVector &state, const Indent &err_ind, const Indent &standard
                            , const std::string &level="error", const std::string &assumption="") const;
    std::string wrap_error(const StateVector &state, const Indent &err_ind
                            , const std::string &level="error", const std::string &assumption="") const;
    void add_error(const StateVector &state, const Indent &err_ind, const Indent &standard
        , const std::string &level="error", const std::string &assumption="");
    void add_error(const StateVector &state, const Indent &err_ind
        , const std::string &level="error", const std::string &assumption="");
public:
    std::string str_stats() const;
    explicit Analyzer(const TokenTable &other, Rules rules);
    explicit operator std::string() const;
    void first_pass();
    void collect_stats();
    void analyze();
    std::string error_messages() const;
//    void pushToken(Token token);
//    void newline(Indent indent);
//    Token &lastToken();
};


#endif //C_FORMAT_CHECKER_ANALYZER_HPP
