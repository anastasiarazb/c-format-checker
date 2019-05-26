#include "token_table.hpp"


void TokenTable::pushToken(const Token &token)
{
    back().push_back(token);
}

void TokenTable::newline(Indent indent, const StateVector &rule_cases)
{
    if (back().empty()) {  // Store only non_empty lines
        pop_back();
    }
    emplace_back(indent);
    back().addState(rule_cases);
}

Token &TokenTable::lastToken()
{
    return back().back();
}

void TokenTable::correctState(const StateVector &rule_cases)
{
    if (back().size() == 1) {
        // We correct state for the first token in line => correct state for the while line
        back().resetState(rule_cases);
    }  // else: do nothing
}
