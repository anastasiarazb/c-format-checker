#ifndef STATE_H
#define STATE_H
#include "frontend/lexer/token.hpp"
#include "frontend/rules/rules.hpp"

class State: std::vector<Token>
{
public:
    Rules::Cases rule;
    State();
};

#endif // STATE_H
