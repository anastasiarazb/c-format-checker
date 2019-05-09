#ifndef STATE_H
#define STATE_H
#include "frontend/lexer/token.hpp"
#include "frontend/rules/rules.hpp"
#include <string>

class State: public std::vector<Token>
{
    Rules::Cases rule = Rules::Cases::BLOCK;
    int level = 0;
public:
    State();
    void set_rule(int level, Rules::Cases rule);
    operator std::string() const;
};

#endif // STATE_H
