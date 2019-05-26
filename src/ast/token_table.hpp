#ifndef C_FORMAT_CHECKER_TOKENTABLE_HPP
#define C_FORMAT_CHECKER_TOKENTABLE_HPP

#include "line.hpp"
#include "state_vector.hpp"
#include "frontend/lexer/token.hpp"
#include "frontend/rules/rules.hpp"

class TokenTable: public std::vector<Line> {
public:
    void pushToken(const Token &token);
    void newline(Indent indent, const StateVector &rule_cases);
    Token &lastToken();
    void correctState(const StateVector &rule_cases);
};


#endif //C_FORMAT_CHECKER_TOKENTABLE_HPP
